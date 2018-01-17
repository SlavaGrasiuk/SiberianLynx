#pragma once

class BaseGameLogic;
class EventManager;
struct SDL_Window;

/*
===============================================================================

		Provides abstraction from software platfrom (e.g. Windows, linux,
	bare-metal). This implementation uses SDL2, so no need to change it for 
	those platforms, who have SDL2.

		Basic inititalization, message pump and shutdown. You must inherit 
	from this class and implement game-specific functions.

===============================================================================
*/
class SimpleEngineApp {
	BaseGameLogic *m_game;
	SDL_Window *m_window;
	EventManager *m_eventManager;
	bool m_isRunning;

public:
	SimpleEngineApp();

	bool Init(int argc, char **argv, const int screenW, const int screenH);
	
	void MainLoop();
	void Shutdown();
	SDL_Window *GetWindow() const;
	int GetExitCode() const;

private:
	void OnMessage();
	void OnUpdate(const float deltaTimeMs);
	void OnRender(const float deltaTimeMs);
	void OnQuit();
	void RegisterEngineEvents();
	void LoadWindowIcon();

protected:
	virtual BaseGameLogic *CreateGameAndView() = 0;
	virtual void RegisterGameEvents();
	virtual const char *GetWindowTitle() = 0;
	virtual unsigned short GetIconResourceID() = 0;
};

extern SimpleEngineApp *g_app;
