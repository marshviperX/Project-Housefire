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


#ifndef HOUSEFIRE_GAMEOBJECT_HPP
#define HOUSEFIRE_GAMEOBJECT_HPP


#include "gameObjectManager.hpp"
#include <vector>


class GameObject : public TypedReferenceCount, public Namable
{
	friend class GameObjectManager;

public:
	
	GameObject();

	GameObject(std::string const& name);

	GameObjectManager* get_manager() const;

	void add_controller(GameObjectControllerPtr const& controller);

	void remove_controller(GameObjectControllerPtr const& controller);

	void add_dependency(GameObjectPtr const& dependency);

	void remove_dependency(GameObjectPtr const& dependency);

	virtual void reset();

	virtual void update(double elapsed);

	static TypeHandle get_class_type();

	static void init_type();

private:

	typedef std::vector< GameObjectControllerPtr > ControllerContainer;
	
	static TypeHandle _type_handle;

	GameObjectManager* _manager;
	ControllerContainer _controllers;
};


inline GameObjectManager* GameObject::get_manager() const {
	return _manager;
}

inline void GameObject::add_dependency(GameObjectPtr const& dependency) {
	_manager->add_object_dependency(GameObjectPtr(this), dependency);
}

inline void GameObject::remove_dependency(GameObjectPtr const& dependency) {
	_manager->remove_object_dependency(GameObjectPtr(this), dependency);
}

inline TypeHandle GameObject::get_class_type() {
	return _type_handle;
}

inline void GameObject::init_type() {
	register_type(_type_handle, "GameObject", TypedReferenceCount::get_class_type(), Namable::get_class_type());
}


#endif // HOUSEFIRE_GAMEOBJECT_HPP
