#pragma once

#include "NumPlayersMenu.h"
#include <kit/controllers.h>

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
	selectedButton = 0;
	playerButtons[selectedButton]->setSelected(true);
	for(int i = 0; i < kit::controllers::getNumControllers(); i++)
	{
		controllerCentered.push_back(true);
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

void NumPlayersMenu::handleEvent(kit::Event const & event)
{
	if(!window.isValid())
	{
		return;
	}
	if(event.type == kit::Event::ControllerAxis)
	{
		auto cae = event.as<kit::ControllerAxisEvent>();
		if(cae.axis == 1)
		{
			if(cae.value < -.5f)
			{
				if(controllerCentered[cae.controller])
				{
					moveSelection(-1);
					controllerCentered[cae.controller] = false;
				}
			}
			else if(cae.value > +.5f)
			{
				if(controllerCentered[cae.controller])
				{
					moveSelection(+1);
					controllerCentered[cae.controller] = false;
				}
			}
			else
			{
				controllerCentered[cae.controller] = true;
			}
		}
	}
	else if(event.type == kit::Event::Keyboard)
	{
		auto ke = event.as<kit::KeyboardEvent>();
		if(ke.pressed)
		{
			if(ke.key == ke.Up)
			{
				moveSelection(-1);
			}
			else if(ke.key == ke.Down)
			{
				moveSelection(+1);
			}
		}
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

void NumPlayersMenu::moveSelection(int offset)
{
	playerButtons[selectedButton]->setSelected(false);
	selectedButton = (selectedButton + offset + playerButtons.size()) % playerButtons.size();
	playerButtons[selectedButton]->setSelected(true);
}
