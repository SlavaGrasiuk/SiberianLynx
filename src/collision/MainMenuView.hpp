#pragma once

#include <UserInterface\HumanView.hpp>
#include <UserInterface\Dialog.hpp>


class Background;


class MainMenuView : public HumanView {
	std::shared_ptr<Background> m_background;
	std::shared_ptr<Dialog> m_mainMenuDialog, m_difficultySelectionMenu;

public:
	MainMenuView();
	~MainMenuView() = default;

private:
	void CreateMainMenu();
	void CreateDifficultySelectionMenu();

	static void MainMenuCallback(DialogEventType nEvent, int nControlID, void *userContext);
	static void DifficultySelectionMenu(DialogEventType nEvent, int nControlID, void *userContext);
};

