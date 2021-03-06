#pragma once

#include "EventManager\EventManager.hpp"

class BaseGameLogic;
class Renderer2D;
class ResourceManager;
struct SDL_Window;
union SDL_Event;

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
	IEventManager *m_eventManager;
	Renderer2D *m_renderer;
	ResourceManager *m_resourceManager;
	SDL_Window *m_window;

	bool m_isRunning, m_quiting;
	int m_windowW, m_windowH;
	int m_exitCode;

public:
	SimpleEngineApp();

	bool Init(int argc, char **argv, const int screenW, const int screenH);
	
	void MainLoop();
	void Shutdown();
	SDL_Window *GetWindow() const;
	int GetWindowWidth() const;
	int GetWindowHeight() const;
	Renderer2D *GetRenderer() const;
	ResourceManager *GetResourceMan() const;
	int GetExitCode() const;
	void AbortGame(const int exitCode = 1);
	IEventManager *GetEventManager() const;

private:
	void OnMessage(const SDL_Event * const msg);
	void OnUpdate(const float deltaTimeMs);
	void OnRender(const float deltaTimeMs);
	void RegisterEngineEvents();
	void LoadWindowIcon();

protected:
	virtual BaseGameLogic *CreateGameAndView() = 0;
	virtual void RegisterGameEvents();
	virtual const char *GetWindowTitle() = 0;
	virtual unsigned short GetIconResourceID() = 0;
};

extern SimpleEngineApp *g_app;
