#pragma once


#include <SimpleEngineApp.hpp>


class CollisionApp : public SimpleEngineApp {
protected:
	virtual BaseGameLogic *CreateGameAndView() override;
	virtual void RegisterGameEvents() override;
	virtual const char *GetWindowTitle() override;
	virtual unsigned short GetIconResourceID() override;
};
