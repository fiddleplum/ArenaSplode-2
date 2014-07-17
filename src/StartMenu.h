#pragma once

#include <kit/window.h>
#include <kit/resources.h>

using namespace kit;

class StartMenu
{
public:
	StartMenu(Ptr<Window> window)
	{
		_window = window;
		for (int i = 2; i <= _maxNumPlayers; i++)
		{
			_playerButtons[i - 2] = _window->addButton();
			_playerButtons[i - 2]->setTexture("art/players_buttons.png");
			_playerButtons[i - 2]->setTextureBounds(Recti::minSize(0, (i - 2) * 128, 256, 128));
		}
	}

	~StartMenu()
	{
		if (!_window.isValid())
		{
			return;
		}
		for (int i = 2; i <= _maxNumPlayers; i++)
		{
			_window->removeWidget(_playerButtons[i - 2]);
		}
	}

	void update()
	{
		if (!_window.isValid())
		{
			return;
		}
		for (int i = 2; i <= _maxNumPlayers; i++)
		{
			if (!_playerButtons[i - 2].isValid())
			{
				continue;
			}
			_window->setWidgetPlacement(_playerButtons[i - 2], Vector2f(.5f, .5f), Vector2f(.5f, .5f), Vector2i(0, (int)(((float)i - 1.0f - _maxNumPlayers / 2.0f) * 128.0f)));
		}
	}

	void setPlayersButtonPressedFunction(std::function<void(int numPlayers)> function)
	{
		_playersButtonPressedFunction = function;
		for (int i = 2; i <= _maxNumPlayers; i++)
		{
			_playerButtons[i - 2]->setPressFunction(std::bind(_playersButtonPressedFunction, i));
		}
	}

private:
	static const int _maxNumPlayers = 4;

	Ptr<Window> _window;
	std::function<void(int)> _playersButtonPressedFunction;
	Ptr<gui::Button> _playerButtons [_maxNumPlayers - 1];
};

