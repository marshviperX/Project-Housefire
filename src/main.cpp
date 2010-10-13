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
PT(AudioManager) audioManager;

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
	NodePath temple = window->load_model(framework.get_models(), "mayantemple");
	// Apply the light to model
	temple.set_light(dlnp);
	// Reparent the model to render
	temple.reparent_to(window->get_render());
	// Apply transforms to the model (scale + position)
	temple.set_scale(5, 5, 5);
//	temple.set_pos(-8, 42, 0);

	// Add a task that updates the audioManager every frame
	PT(AsyncTaskManager) taskMgr = AsyncTaskManager::get_global_ptr();
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
