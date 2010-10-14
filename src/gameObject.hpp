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


#include "housefire.hpp"
#include <namable.h>
#include <time_base.h>
#include <typedReferenceCount.h>
#include <vector>


class GameObject : public TypedReferenceCount, public Namable
{
public:
    
    GameObject();

    GameObject(std::string const& name);

    void add_controller(GameObjectControllerPtr const& controller);

    void remove_controller(GameObjectControllerPtr const& controller);

    virtual void update(Time_Span const& elapsed);

private:

    typedef std::vector< GameObjectControllerPtr > ControllerContainer;
    ControllerContainer _controllers;
};


#endif // HOUSEFIRE_GAMEOBJECT_HPP
