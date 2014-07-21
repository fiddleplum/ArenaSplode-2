#pragma once

#include "NumPlayersMenu.h"

const int maxNumPlayers = 4;

NumPlayersMenu::NumPlayersMenu(Ptr<kit::Window> _window)
{
	window = _window;
	for(int i = 0; i < maxNumPlayers; i++)
	{
		Ptr<kit::gui::Button> button = window->addButton();
		button->setTexture("art/players_buttons.png");
		button->setTextureBounds(Recti::minSize(0, i * 128, 256, 128));
		playerButtons.push_back(button);
	}
}

NumPlayersMenu::~NumPlayersMenu()
{
	if(!window.isValid())
	{
		return;
	}
	for(int i = 0; i < maxNumPlayers; i++)
	{
		window->removeWidget(playerButtons[i]);
	}
}

void NumPlayersMenu::handleEvent(kit::Event const &)
{
	if(!window.isValid())
	{
		return;
	}
}

void NumPlayersMenu::updateWidgets()
{
	for(int i = 0; i < maxNumPlayers; i++)
	{
		if(!playerButtons[i].isValid())
		{
			continue;
		}
		window->setWidgetPlacement(playerButtons[i], Vector2f(.5f, .5f), Vector2f(.5f, .5f), Vector2i(0, (int)(((float)i + .5f - maxNumPlayers / 2.f) * 128.0f)));
	}
}

void NumPlayersMenu::setPlayersButtonPressedFunction(std::function<void(int numPlayers)> function)
{
	playersButtonPressedFunction = function;
	for(int i = 0; i < maxNumPlayers; i++)
	{
		playerButtons[i]->setPressFunction(std::bind(playersButtonPressedFunction, i + 1));
	}
}

