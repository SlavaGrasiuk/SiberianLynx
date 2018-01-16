#pragma once

class BaseGameLogic;
struct SDL_Window;

/*
===============================================================================

		Provides abstraction from software platfrom (e.g. Windows, linux,
	bare-metal). This implementation uses SDL2, so no need to change it for 
	those platforms, who have SDL2.

		Basic inititalization, event loop and shutdown. You must inherit 
	from this class and implement game-specific functions.

===============================================================================
*/
class SimpleEngineApp {
	BaseGameLogic *m_game;
	SDL_Window *m_window;
	//EventManager m_eventManager;

public:
	SimpleEngineApp();
	~SimpleEngineApp();

	bool Init(int argc, char **argv);
	void MainLoop();
	void Shutdown();
	SDL_Window *GetWindow() const;

private:
	void OnMessage();
	void OnUpdate(const float deltaTimeMs);
	void OnRender(const float deltaTimeMs);
	void OnQuit();

protected:
	virtual BaseGameLogic *CreateLogicAndView() = 0;
};

extern SimpleEngineApp *g_app;
