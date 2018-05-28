#include "CollisionApp.hpp"
#include "CollisionLogic.hpp"
#include "MainMenuView.hpp"
#ifdef _WIN32
#include "RC\resource.h"
#endif // _WIN32



/*
==================
CollisionApp::CreateGameAndView
==================
*/
BaseGameLogic * CollisionApp::CreateGameAndView() {
	auto game = new(std::nothrow) CollisionLogic;
	game->Init();

	std::shared_ptr<IGameView> gameMenu = std::make_shared<MainMenuView>();
	game->AddView(gameMenu);

	return game;
}

/*
==================
CollisionApp::GetWindowTitle
==================
*/
const char * CollisionApp::GetWindowTitle() {
	return "Collision";
}

/*
==================
CollisionApp::RegisterGameEvents
==================
*/
void CollisionApp::RegisterGameEvents() {

}

/*
==================
CollisionApp::GetIconResourceID
==================
*/
unsigned short CollisionApp::GetIconResourceID() {
#ifdef _WIN32
	return IDI_ICON1;
#else
	return 0;
#endif // _WIN32
}
