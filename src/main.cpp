#include "pandaFramework.h"
#include "pandaSystem.h"
#include "load_prc_file.h"
#include "ambientLight.h"
#include "directionalLight.h"

#ifdef _MSC_VER
#include <tchar.h>
#endif

PandaFramework framework;

NodePath camera;

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

	//Add ambient light
//	alight = AmbientLight('alight')
//	alight.setColor(VBase4(0.2, 0.2, 0.2, 1))
//	alnp = render.attachNewNode(alight)
//	render.setLight(alnp)

	// Load the environment model
	NodePath environ = window->load_model(framework.get_models(), "mayantemple");
	//Tell it to be lit
	environ.set_light(dlnp);
	// Reparent the model to render
	environ.reparent_to(window->get_render());
	// Apply transforms to the model (scale + position)
	environ.set_scale(5, 5, 5);
//	environ.set_pos(-8, 42, 0);

	//Do the main loop
	framework.main_loop();
	//Close the window
	framework.close_framework();
	return 0;
}
