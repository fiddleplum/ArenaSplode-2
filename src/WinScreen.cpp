#include "WinScreen.h"
#include "Game.h"
#include <kit/math_util.h>
#include <kit/audio.h>

WinScreen::WinScreen(Ptr<kit::Window> _window, std::string const & characterFilename)
{
	window = _window;

	for(int i = 0; i < numSprites; i++)
	{
		sprite[i] = window->addSprite();
		sprite[i]->setTexture("art/characters/" + characterFilename);
		sprite[i]->setTextureBounds(Recti::minSize(0, 0, 64, 64));
		sprite[i]->setPosition(Vector2i(kit::math::random(0, window->getSize()[0]), kit::math::random(0, window->getSize()[1])));
	}
	timeShownSoFar = 0;
	kit::audio::play("sounds/win.ogg");
}

WinScreen::~WinScreen()
{
	if(window.isValid())
	{
		for(int i = 0; i < numSprites; i++)
		{
			window->removeWidget(sprite[i]);
		}
	}
}

void WinScreen::handleEvent(kit::Event const & event)
{
	if(timeShownSoFar > 5.f)
	{
		if(event.type == kit::Event::Keyboard)
		{
			auto ke = event.as<kit::KeyboardEvent>();
			if(ke.pressed && ke.key == ke.Enter)
			{
				game->restart();
			}
		}
		else if(event.type == kit::Event::ControllerButton)
		{
			auto cbe = event.as<kit::ControllerButtonEvent>();
			if(cbe.pressed)
			{
				game->restart();
			}
		}
	}
	if(event.type == kit::Event::Update)
	{
		auto ue = event.as<kit::UpdateEvent>();
		timeShownSoFar += ue.dt;
	}
}