#include "Player.h"
#include "Level.h"
#include "CharacterMenu.h"
#include "Game.h"
#include <kit/gui/viewport.h>
#include <kit/math_util.h>
#include <kit/audio.h>

Player::Player(int _number, Ptr<kit::Window> _window, Ptr<kit::scene::Scene> _scene, Ptr<Level> _level)
{
	number = _number;
	window = _window;
	scene = _scene;
	level = _level;
	camera.setNew(scene);
	camera->setMaxViewSize(600.0f);
	viewport = window->addViewport();
	viewport->setScene(scene);
	viewport->setCamera(camera->getSceneCamera());
	speed = 50.0f;
	looking = false;

	characterMenu.setNew(window);
	characterMenu->setCharacterChosenFunction(std::bind(&Player::characterChosen, this, std::placeholders::_1));
}

Player::~Player()
{
	if(characterMenu.isValid())
	{
		characterMenu.setNull();
	}
	level->removeObject(character);
	if(window.isValid())
	{
		window->removeWidget(viewport);
	}
}

bool Player::hasCharacter() const
{
	return character.isValid();
}

void Player::characterChosen(std::string const & characterFilename)
{
	if(character.isValid())
	{
		level->removeObject(character);
	}
	character.setNew(scene, characterFilename);
	character->setPosition(Vector2f(kit::math::random(0.0f, 1.0f), kit::math::random(0.0f, 1.0f)));
	level->addObject(character);
	kit::audio::play("sound/smw_coin.wav");
	characterMenu.setNull();
	if(characterChosenFunction)
	{
		characterChosenFunction(number);
	}
}

void Player::handleEvent(kit::Event const & event)
{
	if(characterMenu.isValid())
	{
		characterMenu->handleEvent(event);
		return;
	}
}

void Player::handleSceneEvent(kit::Event const & event)
{
	if(!character.isValid())
	{
		return;
	}
	if(number == 0)
	{
		if(event.type == kit::Event::Keyboard)
		{
			kit::KeyboardEvent const & ke = event.as<kit::KeyboardEvent>();
			if(ke.key == kit::KeyboardEvent::A)
			{
				moving[0] -= ke.pressed ? 1 : -1;
			}
			if(ke.key == kit::KeyboardEvent::S)
			{
				moving[1] -= ke.pressed ? 1 : -1;
			}
			if(ke.key == kit::KeyboardEvent::D)
			{
				moving[0] += ke.pressed ? 1 : -1;
			}
			if(ke.key == kit::KeyboardEvent::W)
			{
				moving[1] += ke.pressed ? 1 : -1;
			}
			looking = !moving.isZero();
			lookDirection = moving;
		}
	}
	if(event.type == kit::Event::Update)
	{
		Vector2f velocity = character->getVelocity();
		velocity += Vector2f(moving) * speed * event.as<kit::UpdateEvent>().dt;
		character->setVelocity(velocity);

		if(looking)
		{
			character->setOrientation(std::atan2f(lookDirection[1], lookDirection[0]));
		}
		else if(velocity.normSq() >= .1f)
		{
			character->setOrientation(std::atan2f(velocity[1], velocity[0]));
		}
	}
	else if(event.type == kit::Event::PreRenderUpdate)
	{
		camera->setPosition(character->getPosition());
	}
}

void Player::updateWidgets()
{
	if(game->players.size() == 1)
	{
		bounds.min.set(0, 0);
		bounds.setSize(window->getSize());
	}
	else if(game->players.size() == 2)
	{
		bounds.min.set((int)(number * .5f * window->getSize()[0]), 0);
		bounds.setSize(Vector2i((int)(window->getSize()[0] * .5f), window->getSize()[1]));
	}
	else if(game->players.size() == 3)
	{
		bounds.min.set((int)(number * .333f * window->getSize()[0]), 0);
		bounds.setSize(Vector2i((int)(window->getSize()[0] * .333f), window->getSize()[1]));
	}
	else if(game->players.size() == 4)
	{
		bounds.min.set((int)((number % 2) * .5f * window->getSize()[0]), (int)((number / 2) * .5f * window->getSize()[1]));
		bounds.setSize(Vector2i((int)(window->getSize()[0] * .5f), (int)(window->getSize()[1] * .5f)));
	}
	if(viewport.isValid())
	{
		viewport->setPosition(bounds.min);
		viewport->setMaxSize(bounds.getSize());
	}
	if(characterMenu.isValid())
	{
		characterMenu->updateWidgets(bounds);
	}
}

void Player::setCharacterChosenFunction(std::function<void(int number)> function)
{
	characterChosenFunction = function;
}

