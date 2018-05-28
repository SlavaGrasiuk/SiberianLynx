#include "MainMenuView.hpp"
#include <UserInterface\Background.hpp>
#include <SimpleEngineApp.hpp>
#include "CollisionLogic.hpp"


enum class ButtonID {
	NEW_GAME = 1000,
	EXIT,

	START_EASY,
	START_NORMAL,
	START_HARD,
	BACK
};


/*
==================
MainMenuView::MainMenuView
==================
*/
MainMenuView::MainMenuView() : m_background(new(std::nothrow) Background("textures\\background.png")), m_mainMenuDialog(new(std::nothrow) Dialog), m_difficultySelectionMenu(new(std::nothrow) Dialog) {
	PushElement(m_background);

	CreateMainMenu();
	m_mainMenuDialog->SetVisible(true);
	m_mainMenuDialog->SetZOrder(1);
	m_mainMenuDialog->SetCallback(MainMenuCallback, this);
	PushElement(m_mainMenuDialog);

	CreateDifficultySelectionMenu();
	m_difficultySelectionMenu->SetVisible(false);
	m_difficultySelectionMenu->SetZOrder(1);
	m_difficultySelectionMenu->SetCallback(DifficultySelectionMenu, this);
	PushElement(m_difficultySelectionMenu);
}

/*
==================
MainMenuView::CreateMainMenu
==================
*/
void MainMenuView::CreateMainMenu() {
	m_mainMenuDialog->AddButton(int(ButtonID::NEW_GAME), 40, "textures\\new_game_btn.png");
	m_mainMenuDialog->AddButton(int(ButtonID::EXIT), 85, "textures\\exit_btn.png");
}

/*
==================
MainMenuView::CreateDifficultySelectionMenu
==================
*/
void MainMenuView::CreateDifficultySelectionMenu() {
	m_difficultySelectionMenu->AddButton(int(ButtonID::START_EASY), 40, "textures\\easy_btn.png");
	m_difficultySelectionMenu->AddButton(int(ButtonID::START_NORMAL), 85, "textures\\normal_btn.png");
	m_difficultySelectionMenu->AddButton(int(ButtonID::START_HARD), 130, "textures\\hard_btn.png");
	m_difficultySelectionMenu->AddButton(int(ButtonID::BACK), 175, "textures\\back_btn.png");
}

/*
==================
MainMenuView::MainMenuCallback
==================
*/
void MainMenuView::MainMenuCallback(DialogEventType nEvent, int nControlID, void *userContext) {
	auto mainMenuView = static_cast<MainMenuView*>(userContext);

	if (nEvent == DialogEventType::BUTTON_CLICKED) {
		switch (ButtonID(nControlID)) {
			case ButtonID::NEW_GAME:
				mainMenuView->m_mainMenuDialog->SetVisible(false);
				mainMenuView->m_difficultySelectionMenu->SetVisible(true);
				break;

			case ButtonID::EXIT:
				g_app->AbortGame(0);
				break;

			default:
				break;
		}
	}
}

/*
==================
MainMenuView::DifficultySelectionMenu
==================
*/
void MainMenuView::DifficultySelectionMenu(DialogEventType nEvent, int nControlID, void *userContext) {
	auto mainMenuView = static_cast<MainMenuView*>(userContext);

	if (nEvent == DialogEventType::BUTTON_CLICKED) {
		switch (ButtonID(nControlID)) {
			case ButtonID::START_EASY: {
					std::shared_ptr<EventDataStartGame> eventDataStartGame(new(std::nothrow) EventDataStartGame(GameDifficulty::EASY));
					g_app->GetEventManager()->QueueEvent(eventDataStartGame);
				}
				break;

			case ButtonID::START_NORMAL: {
					std::shared_ptr<EventDataStartGame> eventDataStartGame(new(std::nothrow) EventDataStartGame(GameDifficulty::NORMAL));
					g_app->GetEventManager()->QueueEvent(eventDataStartGame);
				}
				break;

			case ButtonID::START_HARD: {
					std::shared_ptr<EventDataStartGame> eventDataStartGame(new(std::nothrow) EventDataStartGame(GameDifficulty::HARD));
					g_app->GetEventManager()->QueueEvent(eventDataStartGame);
				}
				break;

			case ButtonID::BACK:
				mainMenuView->m_mainMenuDialog->SetVisible(true);
				mainMenuView->m_difficultySelectionMenu->SetVisible(false);
				break;

			default:
				break;
		}
	}
}
