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


#ifndef HOUSEFIRE_GAMEOBJECTCONTROLLER_HPP
#define HOUSEFIRE_GAMEOBJECTCONTROLLER_HPP


#include "housefire.hpp"


class GameObjectController : public TypedReferenceCount
{
public:

	GameObjectController();

	virtual void update(GameObject* object, double elapsed);

	static TypeHandle get_class_type();

	static void init_type();

private:

	static TypeHandle _type_handle;
};


inline TypeHandle GameObjectController::get_class_type() {
	return _type_handle;
}

inline void GameObjectController::init_type() {
	register_type(_type_handle, "GameObjectController", TypedReferenceCount::get_class_type());
}


#endif // HOUSEFIRE_GAMEOBJECTCONTROLLER_HPP
