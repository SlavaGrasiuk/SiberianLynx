#include <SimpleEngineEntry.hpp>
#include "CollisionApp.hpp"


CollisionApp g_collisionApp;

extern "C" int SDL_main(int argc, char *argv[]);

/*
==================
main
==================
*/
int main(int argc, char *argv[]) {
	return SimpleEngine(argc, argv, 240, 320);
}
