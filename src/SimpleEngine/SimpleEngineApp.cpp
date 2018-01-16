#include "include\SimpleEngineApp.hpp"


SimpleEngineApp *g_app;


/*
==================
SimpleEngineApp::SimpleEngineApp
==================
*/
SimpleEngineApp::SimpleEngineApp(): m_game(nullptr) {
	g_app = this;
}

/*
==================
SimpleEngineApp::~SimpleEngineApp
==================
*/
SimpleEngineApp::~SimpleEngineApp() {

}

/*
==================
SimpleEngineApp::Init
==================
*/
bool SimpleEngineApp::Init(int argc, char ** argv) {
	return false;
}

/*
==================
SimpleEngineApp::MainLoop
==================
*/
void SimpleEngineApp::MainLoop() {

}

/*
==================
SimpleEngineApp::Shutdown
==================
*/
void SimpleEngineApp::Shutdown() {

}

/*
==================
SimpleEngineApp::GetWindow
==================
*/
SDL_Window * SimpleEngineApp::GetWindow() const {
	return m_window;
}
