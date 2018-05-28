#pragma once

#include <UserInterface\HumanView.hpp>

class Background;
class ScoreCounter;

class CollisionView : public HumanView {
	std::shared_ptr<Background> m_background, m_scoreCounter;

public:
	CollisionView();
	~CollisionView();
};
