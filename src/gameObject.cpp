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


#include "gameObject.hpp"
#include "gameObjectController.hpp"
#include <algorithm>


TypeHandle GameObject::_type_handle;


GameObject::GameObject()
: _manager(0) {
}

GameObject::GameObject(std::string const& name)
: Namable(name),
_manager(0) {
}

void GameObject::add_controller(GameObjectControllerPtr const& controller) {
	assert(controller != 0);

	ControllerContainer::iterator itr = std::find(_controllers.begin(), _controllers.end(), controller);
	if (itr != _controllers.end()) {
		return;
	}

	_controllers.push_back(controller);
}

void GameObject::remove_controller(GameObjectControllerPtr const& controller) {
	assert(controller != 0);
	ControllerContainer::iterator itr = std::remove(_controllers.begin(), _controllers.end(), controller);
	_controllers.erase(itr, _controllers.end());
}

void GameObject::reset() {
}

void GameObject::update(double elapsed) {
	for (ControllerContainer::iterator itr = _controllers.begin(), end = _controllers.end(); itr != end; ++itr) {
		(*itr)->update(this, elapsed);
	}
}
