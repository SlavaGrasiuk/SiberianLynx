#include "include\SimpleEngineApp.hpp"
#include "include\BaseGameLogic.hpp"
#include "EventManager\EventManager_local.hpp"
#include <SDL2\SDL.h>
#include <chrono>
#include <cstring>
#ifdef _WIN32
#include <Windows.h>
#endif


SimpleEngineApp *g_app;


#define SAFE_DELETE(ptr)	\
	if(ptr) {				\
		delete ptr;			\
		ptr = nullptr;		\
	}

/*
==================
SimpleEngineApp::SimpleEngineApp
==================
*/
SimpleEngineApp::SimpleEngineApp(): m_game(nullptr), m_eventManager(nullptr), m_isRunning(false), m_quiting(false), m_exitCode(1) {
	g_app = this;
}

/*
==================
SimpleEngineApp::Init
==================
*/
bool SimpleEngineApp::Init(int argc, char ** argv, const int screenW, const int screenH) {

	//NOTE here we can check system hardware resources

	RegisterEngineEvents();
	RegisterGameEvents();

	//NOTE here we can initialize resource cache, strings and scripting

	m_eventManager = new EventManager;
	if (m_eventManager == nullptr) {
		return false;
	}

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return false;
	}

	m_window = SDL_CreateWindow(GetWindowTitle(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenW, screenH, SDL_WINDOW_SHOWN);
	if (m_window == nullptr) {
		return false;
	}
	
	SDL_GetWindowSize(m_window, &m_windowW, &m_windowH);		//Window can have size less than requested one.
	LoadWindowIcon();

	//NOTE create and initialize renderer here

	m_game = CreateGameAndView();
	if (m_game == nullptr) {
		return false;
	}

	m_isRunning = true;
	
	return true;
}

/*
==================
SimpleEngineApp::LoadWindowIcon

	Performs OS Windows specific task - setting window icon
==================
*/
void SimpleEngineApp::LoadWindowIcon() {
#if defined(_WIN32)
	const unsigned int mask_r = 0x00ff0000;
	const unsigned int mask_g = 0x0000ff00;
	const unsigned int mask_b = 0x000000ff;
	const unsigned int mask_a = 0xff000000;
	const int res_id = 101;
	const int size = 32;
	const int bpp = 32;

	HICON icon = (HICON)LoadImage(
		GetModuleHandle(NULL),
		MAKEINTRESOURCE(GetIconResourceID()),
		IMAGE_ICON,
		size, size,
		LR_SHARED
	);

	if (icon) {
		ICONINFO ici;

		if (GetIconInfo(icon, &ici)) {
			HDC dc = CreateCompatibleDC(NULL);

			if (dc) {
				SDL_Surface* surface = SDL_CreateRGBSurface(0, size, size, bpp, mask_r, mask_g, mask_b, mask_a);

				if (surface) {
					BITMAPINFO bmi;
					bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
					bmi.bmiHeader.biWidth = size;
					bmi.bmiHeader.biHeight = -size;
					bmi.bmiHeader.biPlanes = 1;
					bmi.bmiHeader.biBitCount = bpp;
					bmi.bmiHeader.biCompression = BI_RGB;
					bmi.bmiHeader.biSizeImage = 0;

					SelectObject(dc, ici.hbmColor);
					GetDIBits(dc, ici.hbmColor, 0, size, surface->pixels, &bmi, DIB_RGB_COLORS);
					SDL_SetWindowIcon(m_window, surface);
					SDL_FreeSurface(surface);
				}
				DeleteDC(dc);
			}
			DeleteObject(ici.hbmColor);
			DeleteObject(ici.hbmMask);
		}
		DestroyIcon(icon);
	}
#endif
}

/*
==================
SimpleEngineApp::MainLoop

	SimpleEngine make use of the following convention: 
		- events, pumped from operating system are called "messages"
		- events, processed by engine's EventManager are called "events"

	This function works with the operating system's message pump.
==================
*/
void SimpleEngineApp::MainLoop() {
	using namespace std::chrono;

	SDL_Event msg;
	memset(&msg, 0, sizeof msg);
	float frameTimeMs = 1.0f / 60.0f;
	auto prevFrameEndTimePoint = high_resolution_clock::now();

	while (msg.type != SDL_QUIT) {
		const bool gotMsg = SDL_PollEvent(&msg);	// Use SDL_PollEvent() so we can use idle time to render the scene. 

		if (gotMsg) {
			OnMessage(&msg);
		} else {					// Update and render a frame during idle time (no messages are waiting)
			OnUpdate(frameTimeMs);
			OnRender(frameTimeMs);

			const auto thisFrameEndTimePoint = high_resolution_clock::now();
			frameTimeMs = duration<float, std::milli>(thisFrameEndTimePoint - prevFrameEndTimePoint).count();
			prevFrameEndTimePoint = thisFrameEndTimePoint;
		}
	}
}

/*
==================
SimpleEngineApp::Shutdown
==================
*/
void SimpleEngineApp::Shutdown() {
	SAFE_DELETE(m_game)

	SAFE_DELETE(m_eventManager)

	//SAFE_DELETE(m_renderer)

	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

/*
==================
SimpleEngineApp::GetWindow
==================
*/
SDL_Window * SimpleEngineApp::GetWindow() const {
	return m_window;
}

/*
==================
SimpleEngineApp::GetExitCode
==================
*/
int SimpleEngineApp::GetExitCode() const {
	return m_exitCode;
}

/*
==================
SimpleEngineApp::AbortGame
==================
*/
void SimpleEngineApp::AbortGame(const int exitCode) {
	m_exitCode = exitCode;
	m_quiting = true;
}

/*
==================
SimpleEngineApp::GetEventManager
==================
*/
IEventManager *SimpleEngineApp::GetEventManager() const {
	return m_eventManager;
}

/*
==================
SimpleEngineApp::OnMessage
==================
*/
void SimpleEngineApp::OnMessage(const SDL_Event * const msg) {
	switch (msg->type) {
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEMOTION:
		case SDL_KEYDOWN:
		case SDL_KEYUP:
		case SDL_FINGERDOWN:
		case SDL_FINGERUP:
		case SDL_FINGERMOTION:
			{
				if (m_game) {
					//SDL2 has already converted messages into the platform-independent format.

					//Send message to the game views in the reverse order.
					for (auto it = m_game->GetGameViewList().rbegin(); it != m_game->GetGameViewList().rend(); it++) {
						if ((*it)->OnMsgProc(msg)) {
							break;		// break from for loop
						}
					}
				}
			}
			break;

		default:
			break;
	}
}

/*
==================
SimpleEngineApp::OnUpdate
==================
*/
void SimpleEngineApp::OnUpdate(const float deltaTimeMs) {
	if (m_quiting) {
		SDL_Event msg;
		msg.type = SDL_QUIT;
		SDL_PushEvent(&msg);
	}

	if (m_game) {
		m_eventManager->Update(20.0f);

		m_game->OnUpdate(deltaTimeMs);
	}
}

/*
==================
SimpleEngineApp::OnRender
==================
*/
void SimpleEngineApp::OnRender(const float deltaTimeMs) {
	for (auto &view : m_game->GetGameViewList()) {
		view->VOnRender(deltaTimeMs);
	}

	m_game->RenderDiagnostic();

	//Call PresentScene on renderer. Execution will be locket until vsync.
}

/*
==================
SimpleEngineApp::RegisterEngineEvents
==================
*/
void SimpleEngineApp::RegisterEngineEvents() {

}

/*
==================
SimpleEngineApp::RegisterGameEvents
==================
*/
void SimpleEngineApp::RegisterGameEvents() {
	//empty in most circumstances
}
