#include "pandaFramework.h"
#include "pandaSystem.h"

PandaFramework framework;

int main(int argc, char *argv[]) {
	//Open a new window framework
	framework.open_framework(argc, argv);
	//Set the window title
	framework.set_window_title("Panda3d first program");
	//Open the created window
	WindowFramework *window = framework.open_window();

	//Default trackballing
	window->setup_trackball();

	//Room for actual code
	// Load the environment model
	NodePath environ = window->load_model(framework.get_models(), "models/environment");
	// Reparent the model to render
	environ.reparent_to(window->get_render());
	// Apply transforms to the model (scale + position)
//	environ.set_scale(0.25, 0.25, 0.25);
//	environ.set_pos(-8, 42, 0);

	//Do the main loop
	framework.main_loop();
	//Close the window
	framework.close_framework();
	return 0;
}
