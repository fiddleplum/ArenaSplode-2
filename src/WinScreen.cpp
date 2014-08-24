#include "WinScreen.h"
#include "Game.h"
#include <kit/math_util.h>
#include <kit/audio.h>

WinScreen::WinScreen(Ptr<kit::Window> _window, std::string const & characterFilename)
{
	window = _window;

	sprite = window->addSprite();
	sprite->setTexture("art/characters/" + characterFilename);
	int textureSize = sprite->getTexture()->getSize()[0];
	sprite->setTextureBounds(Recti::minSize(0, 0, textureSize, textureSize));
	sprite->setScale(4.f * (64.f / textureSize));
	sprite->setPosition(Vector2i((window->getSize()[0] - sprite->getBounds().getSize()[0]) / 2, (window->getSize()[1] - sprite->getBounds().getSize()[1])  /2));

	timeShownSoFar = 0;
	kit::audio::play("sounds/win.ogg");
}

WinScreen::~WinScreen()
{
	if(window.isValid())
	{
		window->removeWidget(sprite);
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

