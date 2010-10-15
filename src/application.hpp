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


#ifndef HOUSEFIRE_APPLICATION_HPP
#define HOUSEFIRE_APPLICATION_HPP


#include "housefire.hpp"
#include <configPage.h>
#include <genericAsyncTask.h>
#include <windowFramework.h>

#ifdef HOUSEFIRE_PLATFORM_WINDOWS
#	include <tchar.h>
#endif


class Application
{
	Application(Application const&);
	Application& operator=(Application const&);

public:

	static Application* instance();

	ConfigPage* get_config_page() const;

	PandaFramework* get_framework() const;

	WindowFramework* get_window() const;

	PointerTo< AudioManager > const& get_audio_manager() const;

	GameObjectManagerPtr const& get_object_manager() const;

#ifdef HOUSEFIRE_PLATFORM_WINDOWS
	bool initiate(LPTSTR commandLine);
#endif

	bool initiate(int argc, char** argv);

	void terminate();

	void run();

private:

	Application();

	bool initiate_engine(int argc, char** argv);

	void terminate_engine();

	bool load_assets();

	void unload_assets();

	void update();

	static AsyncTask::DoneStatus update_task_callback(GenericAsyncTask* task, void* data);

private:

	ConfigPage* _config_page;
	PandaFramework* _framework;
	WindowFramework* _window;
	GameObjectManagerPtr _object_manager;
	PointerTo< AudioManager > _audio_manager;
	PointerTo< GenericAsyncTask > _update_task;
	PointerTo< AudioSound > _background_music;
};


inline ConfigPage* Application::get_config_page() const {
	return _config_page;
}

inline PandaFramework* Application::get_framework() const {
	return _framework;
}

inline WindowFramework* Application::get_window() const {
	return _window;
}

inline PointerTo< AudioManager > const& Application::get_audio_manager() const {
	return _audio_manager;
}

inline GameObjectManagerPtr const& Application::get_object_manager() const {
	return _object_manager;
}


#endif // HOUSEFIRE_APPLICATION_HPP
