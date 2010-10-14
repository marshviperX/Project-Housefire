//
// Project Housefire
// Copyright (C) 2010 LAMMJohnson
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//


#include "gameObjectManager.hpp"
#include "gameObject.hpp"
#include <algorithm>


TypeHandle GameObjectManager::_type_handle;


GameObjectManager::GameObjectManager()
: _objects_need_sorting(false) {
}

GameObjectPtr GameObjectManager::find_object(std::string const& name) const {
	ObjectNameMap::const_iterator entry_itr = _objects_by_name.find(name);
	return (entry_itr != _objects_by_name.end()) ? entry_itr->second : GameObjectPtr(0);
}

void GameObjectManager::add_object(GameObjectPtr const& object) {
	assert((object != 0) && object->get_manager() == 0);
	assert(_objects_by_name.find(object->get_name()) == _objects_by_name.end());

	std::pair< std::string, GameObjectPtr > name_entry(object->get_name(), object);
	_objects_by_name.insert(name_entry);
	
	std::pair< GameObject*, ObjectNode > node_entry(object.p(), ObjectNode());
	_object_nodes.insert(node_entry);

	_objects_to_reset.push_back(object.p());
	_objects_need_sorting = true;

	object->_manager = this;
}

void GameObjectManager::remove_object(GameObjectPtr const& object) {
	assert((object != 0) && (object->get_manager() == this));

	// make sure object is removed from reset list
	ObjectContainer::iterator reset_itr = std::remove(_objects_to_reset.begin(), _objects_to_reset.end(), object.p());
	_objects_to_reset.erase(reset_itr, _objects_to_reset.end());

	// remove the object from the dependency graph
	ObjectNodeMap::iterator node_entry_itr = _object_nodes.find(object.p());

	// remove dependency edges from object's dependents
	ObjectContainer& dependents = node_entry_itr->second.dependents;
	for (ObjectContainer::iterator object_itr = dependents.begin(), object_end_itr = dependents.end(); object_itr != object_end_itr; ++object_itr) {
		ObjectNodeMap::iterator entry_itr = _object_nodes.find(*object_itr);
		ObjectContainer::iterator dependency_itr = std::remove(entry_itr->second.dependencies.begin(), entry_itr->second.dependencies.end(), object.p());
		entry_itr->second.dependencies.erase(dependency_itr, entry_itr->second.dependencies.end());
	}

	// remove dependent edges from object's dependencies
	ObjectContainer& dependencies = node_entry_itr->second.dependencies;
	for (ObjectContainer::iterator object_itr = dependencies.begin(), object_end_itr = dependencies.end(); object_itr != object_end_itr; ++object_itr) {
		ObjectNodeMap::iterator entry_itr = _object_nodes.find(*object_itr);
		ObjectContainer::iterator dependent_itr = std::remove(entry_itr->second.dependents.begin(), entry_itr->second.dependents.end(), object.p());
		entry_itr->second.dependents.erase(dependent_itr, entry_itr->second.dependents.end());
	}

	_object_nodes.erase(node_entry_itr);
	_objects_by_name.erase(object->get_name());
	_objects_need_sorting = true;

	object->_manager = 0;
}

void GameObjectManager::add_object_dependency(GameObjectPtr const& object, GameObjectPtr const& dependency) {
	assert((object != 0) && (object->get_manager() == this));
	assert((dependency != 0) && (dependency->get_manager() == this));

	// add dependency edge to the graph
	ObjectNodeMap::iterator object_entry_itr = _object_nodes.find(object.p());
	ObjectContainer& dependencies = object_entry_itr->second.dependencies;
	assert(std::find(dependencies.begin(), dependencies.end(), dependency.p()) == dependencies.end());
	dependencies.push_back(dependency.p());

	// add dependent edge to the graph
	ObjectNodeMap::iterator dependency_entry_itr = _object_nodes.find(dependency.p());
	ObjectContainer& dependents = dependency_entry_itr->second.dependents;
	assert(std::find(dependents.begin(), dependents.end(), object.p()) == dependents.end());
	dependents.push_back(object.p());

	_objects_need_sorting = true;
}

void GameObjectManager::remove_object_dependency(GameObjectPtr const& object, GameObjectPtr const& dependency) {
	assert((object != 0) && (object->get_manager() == this));
	assert((dependency != 0) && (dependency->get_manager() == this));
	
	// add dependency edge to the graph
	ObjectNodeMap::iterator object_entry_itr = _object_nodes.find(object.p());
	ObjectContainer& dependencies = object_entry_itr->second.dependencies;
	ObjectContainer::iterator dependency_itr = std::remove(dependencies.begin(), dependencies.end(), dependency.p());
	dependencies.erase(dependency_itr, dependencies.end());

	// add dependent edge to the graph
	ObjectNodeMap::iterator dependency_entry_itr = _object_nodes.find(dependency.p());
	ObjectContainer& dependents = dependency_entry_itr->second.dependents;
	ObjectContainer::iterator dependent_itr  = std::remove(dependents.begin(), dependents.end(), object.p());
	dependents.erase(dependent_itr, dependencies.end());

	_objects_need_sorting = true;
}

void GameObjectManager::update(Time_Span const& elapsed) {
	if (!_objects_to_reset.empty()) {
		reset_pending_objects();
	}
	
	if (_objects_need_sorting) {
		topologically_sort_objects();
		_objects_need_sorting = false;
	}

	for (ObjectContainer::iterator itr = _ordered_objects.begin(), end = _ordered_objects.end(); itr != end; ++itr) {
		(*itr)->update(elapsed);
	}
}

void GameObjectManager::reset_pending_objects() {
	// Reset any pending game objects. Note that this method is reentrant:
	// additional game objects can be added to this manager from within
	// the reset method of concrete GameObjects.

	while (!_objects_to_reset.empty()) {
		GameObject* object = _objects_to_reset.back();
		_objects_to_reset.pop_back();
		object->reset();
	}
}

void GameObjectManager::topologically_sort_objects() {
	// Perform a depth-first traversal over the dependency graph, inserting objects
	// into the ordered set in reverse order.

	_ordered_objects.clear();
	_ordered_objects.reserve(_object_nodes.size());

	for (ObjectNodeMap::iterator itr = _object_nodes.begin(), end = _object_nodes.end(); itr != end; ++itr) {
		itr->second.visited = false;
	}

	for (ObjectNodeMap::iterator itr = _object_nodes.begin(), end = _object_nodes.end(); itr != end; ++itr) {
		if (itr->second.dependencies.empty()) {
			visit_and_insert_dependents(itr->first, itr->second);
		}
	}

	std::reverse(_ordered_objects.begin(), _ordered_objects.end());
}


void GameObjectManager::visit_and_insert_dependents(GameObject* object, ObjectNode& node) {
	if (!node.visited) {
		node.visited = true;

		for (ObjectContainer::iterator object_itr = node.dependents.begin(), object_end_itr = node.dependents.end(); object_itr != object_end_itr; ++object_itr) {
			ObjectNodeMap::iterator node_entry_itr = _object_nodes.find(*object_itr);
			visit_and_insert_dependents(node_entry_itr->first, node_entry_itr->second);
		}

		_ordered_objects.push_back(object);
	}
}
