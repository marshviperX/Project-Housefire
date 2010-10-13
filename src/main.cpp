#include "pandaFramework.h"
#include "pandaSystem.h"
#include "load_prc_file.h"
#include "asyncTaskManager.h"
#include "ambientLight.h"
#include "directionalLight.h"
#include "audioManager.h"
#include <vector>
#include "audioSound.h"

#ifdef _MSC_VER
#include <tchar.h>
#endif

PandaFramework framework;
NodePath camera;
PT(AudioManager) audioManager;

//Vector to hold multiple model files
vector<NodePath> models;

double rotateTemplesX = 0;

AsyncTask::DoneStatus rotate_temples_task(GenericAsyncTask* task, void* data){
	for (int i = 0; i < models.size(); i++) {
		models[i].set_hpr(rotateTemplesX, 0, 0);
	}

	if (rotateTemplesX >= 360) {
		rotateTemplesX = 0;
	} else {
		rotateTemplesX++;
	}

	return AsyncTask::DS_cont;
}

AsyncTask::DoneStatus audiomanager_update_task(GenericAsyncTask* task, void* data){
	audioManager->update();
	return AsyncTask::DS_cont;
}

#ifdef _MSC_VER
int WINAPI _tWinMain(HINSTANCE instance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd) {
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
	WindowFramework *window = framework.open_window();

	//Get camera node from window
	camera = window->get_camera_group();

	//Default trackballing
	window->setup_trackball();

	//Add a spotlight
	PT(DirectionalLight) d_light;
	d_light = new DirectionalLight("my d_light");
	NodePath dlnp = window->get_render().attach_new_node(d_light);

	// Load the temple model
	models.push_back(window->load_model(framework.get_models(), "mayantemple"));
	models.push_back(window->load_model(framework.get_models(), "mayantemple"));
	models.push_back(window->load_model(framework.get_models(), "mayantemple"));

	//For each model file in the vector
	for (int i = 0; i < models.size(); i++) {
		// Apply the light to models
		models[i].set_light(dlnp);
		// Reparent the model to render in the window
		models[i].reparent_to(window->get_render());
		// Apply transforms to the model (scale + position)
		models[i].set_scale(5, 5, 5);
		models[i].set_pos(-10 + ( i * 150), 0, 0);
	}
	NodePath temple = window->load_model(framework.get_models(), "mayantemple");
	// Apply the light to model
	temple.set_light(dlnp);

	// Add a task that updates the audioManager every frame
	PT(AsyncTaskManager) taskMgr = AsyncTaskManager::get_global_ptr();
	
	PT(GenericAsyncTask) rotateTemplesTask = new GenericAsyncTask("RotateTemplesTask", &rotate_temples_task, (void*) NULL);
	taskMgr->add(rotateTemplesTask);
	
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
