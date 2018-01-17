#include "include\SimpleEngineApp.hpp"
#include <SDL2\SDL.h>
#include <chrono>
#ifdef _WIN32
#include <Windows.h>
#endif


SimpleEngineApp *g_app;


/*
==================
SimpleEngineApp::SimpleEngineApp
==================
*/
SimpleEngineApp::SimpleEngineApp(): m_game(nullptr), m_eventManager(nullptr), m_isRunning(false) {
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

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return false;
	}

	m_window = SDL_CreateWindow(GetWindowTitle(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenW, screenH, SDL_WINDOW_SHOWN);
	if (m_window == nullptr) {
		return false;
	}

	LoadWindowIcon();

	//NOTE create and initialize render here

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
		- events, processed by engine's EvemtManager are called "events"

	This function works with the operating system's message pump.
==================
*/
void SimpleEngineApp::MainLoop() {
	SDL_Event msg;

	while (msg.type != SDL_QUIT) {
		const bool gotMsg = SDL_PollEvent(&msg);

		if (gotMsg) {

		} else {

		}
	}
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

/*
==================
SimpleEngineApp::GetExitCode
==================
*/
int SimpleEngineApp::GetExitCode() const {
	return 0;
}

/*
==================
SimpleEngineApp::OnMessage
==================
*/
void SimpleEngineApp::OnMessage() {

}

/*
==================
SimpleEngineApp::OnUpdate
==================
*/
void SimpleEngineApp::OnUpdate(const float deltaTimeMs) {

}

/*
==================
SimpleEngineApp::OnRender
==================
*/
void SimpleEngineApp::OnRender(const float deltaTimeMs) {

}

/*
==================
SimpleEngineApp::OnQuit
==================
*/
void SimpleEngineApp::OnQuit() {

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
