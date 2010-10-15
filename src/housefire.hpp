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


#ifndef HOUSEFIRE_HPP
#define HOUSEFIRE_HPP


#if defined(__APPLE__) || defined(__APPLE_CC__)
#   error "Apple platforms not yet supported, should include TargetConditionals.h and perform full platform detection."
#elif defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WIN64__)
#   define HOUSEFIRE_PLATFORM_WINDOWS
#else
#   define HOUSEFIRE_PLATFORM_LINUX
#endif

#if defined(__GNUC__)
#   define HOUSEFIRE_COMPILER_GCC
#elif defined(_MSC_VER)
#   define HOUSEFIRE_COMPILER_MSVC
#endif


#include <pandaFramework.h>
#include <pandaSystem.h>
#include <pandabase.h>


class Application;
class GameObject;
class GameObjectController;
class GameObjectManager;

typedef PointerTo< GameObject > GameObjectPtr;
typedef PointerTo< GameObjectController > GameObjectControllerPtr;
typedef PointerTo< GameObjectManager > GameObjectManagerPtr;


#endif // HOUSEFIRE_HPP
