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


#include "housefire.hpp"

#ifdef HOUSEFIRE_PLATFORM_WINDOWS
#   include <tchar.h>
#endif

#include <pandaFramework.h>
#include <pandaSystem.h>
#include <load_prc_file.h>
#include <asyncTaskManager.h>
#include <ambientLight.h>
#include <directionalLight.h>
#include <audioManager.h>
#include <nodePathCollection.h>
#include <audioSound.h>

PandaFramework framework;
NodePath camera;
WindowFramework *window;
PT(AudioManager) audioManager;

double rotateCameraX = 0;

AsyncTask::DoneStatus rotate_camera_task(GenericAsyncTask* task, void* data){	
	camera.set_hpr(rotateCameraX, 0, 0);

	rotateCameraX >= 360 ? rotateCameraX = 0 : rotateCameraX++;

	return AsyncTask::DS_cont;
}

AsyncTask::DoneStatus audiomanager_update_task(GenericAsyncTask* task, void* data){
	audioManager->update();
	return AsyncTask::DS_cont;
}

#ifdef HOUSEFIRE_PLATFORM_WINDOWS
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd) {
    // TODO: convert lpCmdLine into standard argc/argv pair
    int argc = 0;
    char** argv = 0;
#else
int main(int argc, char *argv[]) {
#endif

	//Load our settings file
	load_prc_file("housefire.prc");

	//Open a new window framework
	framework.open_framework(argc, argv);
	//Set the window title
	framework.set_window_title("Project Housefire Rendering/Camera Test");
	//Open the created window
	window = framework.open_window();

	//Get camera node from window
	camera = window->get_camera_group();

	//Default trackballing
	window->setup_trackball();

	//Add a spotlight
	PT(DirectionalLight) d_light;
	d_light = new DirectionalLight("my d_light");
	NodePath dlnp = window->get_render().attach_new_node(d_light);

	// Load the temple model in 3 instances
	for (int i = 0; i < 20; i++) {
		window->load_model(framework.get_models(), "mayantemple");
	}

	//Access to all models
	NodePathCollection children = framework.get_models().get_children();

	//Apply the following to all models
	for (int i = 0 ; i < children.size() ; i++) {
		children[i].set_scale(7.0f, 7.0f, 7.0f);				//Scale it up
		children[i].set_pos(300.0f * sinf(i), 300.0f * cosf(i), 0); 	//Apply positioning. Just split the mup a bit so that they can all be viewed.
		children[i].set_light(dlnp);				//Apply lighting
		children[i].reparent_to(window->get_render());		//Apply it to the window for rendering
	}
	NodePath temple = window->load_model(framework.get_models(), "mayantemple");
	// Apply the light to model
	temple.set_light(dlnp);

	PT(AsyncTaskManager) taskMgr = AsyncTaskManager::get_global_ptr();
	
//	PT(GenericAsyncTask) rotateCameraTask = new GenericAsyncTask("RotateCameraTask", &rotate_camera_task, (void*) NULL);
//	taskMgr->add(rotateCameraTask);
	
	// Add a task that updates the audioManager every frame
	PT(GenericAsyncTask) audioManagerUpdateTask = new GenericAsyncTask("AudioManagerUpdateTask", &audiomanager_update_task, (void*) NULL);
	taskMgr->add(audioManagerUpdateTask);

	// Play hurr.ogg
	audioManager = AudioManager::create_AudioManager();
	PT(AudioSound) hurr = audioManager->get_sound("ambience.ogg");

	audioManager->set_volume(1.0f);
	hurr->set_loop(true);
	hurr->play();

	//Do the main loop
	framework.main_loop();
	//Close the window
	framework.close_framework();
	return 0;
}
