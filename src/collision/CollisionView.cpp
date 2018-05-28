#include "CollisionView.hpp"
#include <UserInterface\Background.hpp>
#include "ScoreCounter.hpp"



/*
==================
CollisionView::CollisionView
==================
*/
CollisionView::CollisionView() : m_background(new(std::nothrow) Background("textures\\game_background.png")), m_scoreCounter(new(std::nothrow) ScoreCounter(98, 12)) {
	PushElement(m_background);
	PushElement(m_scoreCounter);

}

/*
==================
CollisionView::~CollisionView
==================
*/
CollisionView::~CollisionView() {

}
