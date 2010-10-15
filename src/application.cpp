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


#include "application.hpp"
#include "gameObjectManager.hpp"
#include <asyncTaskManager.h>
#include <clockObject.h>
#include <load_prc_file.h>


Application::Application()
: _config_page(0),
_framework(0),
_window(0) {
}

Application* Application::instance() {
	static Application _instance;
	return &_instance;
}

bool Application::initiate(int argc, char** argv) {
	// TODO: should probably clone a default configuration file from the resource
	// directory, copy it to a local user directory (eg. "~/.housefire/housefire.prc")
	// and load the configuration from there.
	_config_page = load_prc_file("housefire.prc");
	if (!_config_page) {
		return false;
	}
	
	_framework = new PandaFramework;
	_framework->open_framework(argc, argv);
	_framework->set_window_title("Project Housefire");

	_window = _framework->open_window();
	if (!_window) {
		return false;
	}
	
	_update_task = new GenericAsyncTask("HousefireUpdateTask", &Application::update_task_callback, this);
	_framework->get_task_mgr().add(_update_task);

	_object_mgr = new GameObjectManager;
	return true;
}

#ifdef HOUSEFIRE_PLATFORM_WINDOWS
bool Application::initiate(LPTSTR command_line) {
	// TODO: convert command_line to char string and tokenize the arguments before
	// passing it into the other overload of initiate
	return initiate(0, 0);
}
#endif

void Application::terminate() {
	_object_mgr.clear();

	if (_update_task) {
		_framework->get_task_mgr().remove(_update_task);
		_update_task.clear();
	}

	delete _framework;
	_framework = 0;
	_window = 0;
	
	if (_config_page) {
		unload_prc_file(_config_page);
		_config_page = 0;
	}
}

void Application::run() {
	ClockObject* clock = ClockObject::get_global_clock();
	clock->reset();
	_framework->main_loop();
}

void Application::update() {
	ClockObject* clock = ClockObject::get_global_clock();
	double elapsed = clock->get_dt();

	if (_object_mgr) {
		_object_mgr->update(elapsed);
	}
}

AsyncTask::DoneStatus Application::update_task_callback(GenericAsyncTask* task, void* data) {
	Application* app = static_cast< Application* >(data);
	app->update();
	return AsyncTask::DS_cont;
}
