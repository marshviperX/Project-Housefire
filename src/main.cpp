#include "pandaFramework.h"
#include "pandaSystem.h"
#include "load_prc_file.h"
#include "asyncTaskManager.h"
#include "ambientLight.h"
#include "directionalLight.h"
#include "audioManager.h"

#ifdef _MSC_VER
#include <tchar.h>
#endif

PandaFramework framework;
NodePath camera;
NodePath environment;
PT(AudioManager) audioManager;

double rotateEnvironmentX = 0;

AsyncTask::DoneStatus rotate_environment_task(GenericAsyncTask* task, void* data){
	environment.set_hpr(rotateEnvironmentX, 0, 0);

	if (rotateEnvironmentX >= 360) {
		rotateEnvironmentX = 0;
	} else {
		rotateEnvironmentX++;
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

	camera = window->get_camera_group();

	//Default trackballing
	window->setup_trackball();

	//Room for actual code

	//Add a spotlight
	PT(DirectionalLight) d_light;
	d_light = new DirectionalLight("my d_light");
	NodePath dlnp = window->get_render().attach_new_node(d_light);

	// Load the environment model
	environment = window->load_model(framework.get_models(), "mayantemple");
	//Tell it to be lit
	environment.set_light(dlnp);
	// Reparent the model to render
	environment.reparent_to(window->get_render());
	// Apply transforms to the model (scale + position)
	environment.set_scale(5, 5, 5);
	environment.set_pos(0, 42, -14);
	
	PT(AsyncTaskManager) taskMgr = AsyncTaskManager::get_global_ptr();
	
	PT(GenericAsyncTask) rotateEnvironmentTask = new GenericAsyncTask("RotateEnvironmentTask", &rotate_environment_task, (void*) NULL);
	taskMgr->add(rotateEnvironmentTask);

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
