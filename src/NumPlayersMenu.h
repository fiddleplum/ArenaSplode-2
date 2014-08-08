#pragma once

#include <kit/window.h>
#include <kit/ptr.h>

class NumPlayersMenu
{
public:
	NumPlayersMenu(Ptr<kit::Window> window);

	~NumPlayersMenu();

	void handleEvent(kit::Event const & event);

	void updateWidgets();

	void setPlayersButtonPressedFunction(std::function<void(int numPlayers)> function);

private:
	void moveSelection(int offset);

	Ptr<kit::Window> window;
	std::function<void(int)> playersButtonPressedFunction;
	std::vector<Ptr<kit::gui::Button>> playerButtons;
	std::vector<bool> controllerCentered;
	int selectedButton;
};

