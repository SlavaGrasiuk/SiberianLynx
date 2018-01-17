#include "include\SimpleEngineEntry.hpp"
#include "include\SimpleEngineApp.hpp"

/*
==================
SimpleEngine
==================
*/
int SimpleEngine(int argc, char **argv, const int screenX, const int screenY) {
	
	//NOTE Here we can init logging and load options.
	//Temporary set screen resolution from call arguments.

	if (!g_app->Init(argc, argv, screenX, screenY)) {
		return 1;
	}

	g_app->MainLoop();
	g_app->Shutdown();

	return g_app->GetExitCode();
}
