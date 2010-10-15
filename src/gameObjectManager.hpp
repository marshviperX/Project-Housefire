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


#ifndef HOUSEFIRE_GAMEOBJECTMANAGER_HPP
#define HOUSEFIRE_GAMEOBJECTMANAGER_HPP


#include "housefire.hpp"
#include <vector>

#ifdef HOUSEFIRE_COMPILER_MSVC
#   include <unordered_map>
#else
#   include <tr1/unordered_map>
#endif


class GameObjectManager : public TypedReferenceCount
{
public:

	GameObjectManager();

	GameObjectPtr find_object(std::string const& name) const;

	void add_object(GameObjectPtr const& object);

	void remove_object(GameObjectPtr const& object);

	void remove_all_objects();

	void add_object_dependency(GameObjectPtr const& object, GameObjectPtr const& dependency);

	void remove_object_dependency(GameObjectPtr const& object, GameObjectPtr const& dependency);

	void update(double elapsed);

	static TypeHandle get_class_type();

	static void init_type();

private:

	typedef std::vector< GameObject* > ObjectContainer;

	struct ObjectNode
	{
		ObjectContainer dependents;
		ObjectContainer dependencies;
		bool visited;
	};

	typedef std::tr1::unordered_map< GameObject*, ObjectNode > ObjectNodeMap;
	typedef std::tr1::unordered_map< std::string, GameObjectPtr > ObjectNameMap;

	void reset_pending_objects();
	void topologically_sort_objects();
	void visit_and_insert_dependents(GameObject* object, ObjectNode& node);
	
	static TypeHandle _type_handle;

	ObjectNameMap _objects_by_name;
	ObjectNodeMap _object_nodes;
	ObjectContainer _objects_to_reset;
	ObjectContainer _ordered_objects;
	bool _objects_need_sorting;
};


inline TypeHandle GameObjectManager::get_class_type() {
	return _type_handle;
}

inline void GameObjectManager::init_type() {
	register_type(_type_handle, "GameObjectManager", TypedReferenceCount::get_class_type());
}


#endif // HOUSEFIRE_GAMEOBJECTMANAGER_HPP
