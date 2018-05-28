#pragma once


#include "..\Interfaces.hpp"
#include "..\MainLoop\ProcessManager.hpp"
#include <list>


using ScreenElementList = std::list<std::shared_ptr<IScreenElement>>;

constexpr GameViewId INVALID_VIEW_ID = 0xff'ff'ff'ff;


/*
===============================================================================

	Present game to human player. 

===============================================================================
*/
class HumanView : public IGameView {
	ProcessManager m_processManager;		//fora hadling animations, sounds, etc.
	GameViewId m_viewId;
	ActorId m_actorId;
	ScreenElementList m_screenElements;
	bool m_elementAdded;

public:
	HumanView();
	~HumanView();

	virtual void OnRender(const float deltaTimeMs) override;
	virtual GameViewType GetType() override;
	virtual GameViewId GetId() const override;
	virtual void OnAttach(GameViewId vid, ActorId aid) override;
	virtual bool OnMsgProc(const SDL_Event * const msg) override;
	virtual void OnUpdate(const float deltaTimeMs) override;

	virtual void PushElement(std::shared_ptr<IScreenElement> pElement);
	virtual void RemoveElement(std::shared_ptr<IScreenElement> pElement);

protected:
	int m_pointerRadius;
	std::shared_ptr<IPointerHandler> m_mouseHandler;
	std::shared_ptr<ITouchHandler> m_touchHandler;
};

