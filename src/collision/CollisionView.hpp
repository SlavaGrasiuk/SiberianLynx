#pragma once

#include <UserInterface\HumanView.hpp>

class Background;
class ScoreCounter;

class CollisionView : public HumanView {
	std::shared_ptr<Background> m_background;
	std::shared_ptr<ScoreCounter> m_scoreCounter;

public:
	CollisionView();
	~CollisionView();
};
