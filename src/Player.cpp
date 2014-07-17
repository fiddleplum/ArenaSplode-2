#include "Player.h"
#include <kit/gui/viewport.h>
#include <kit/math_util.h>

Player::Player (int number, Ptr<Window> window, Ptr<scene::Scene> scene, Ptr<Level> level)
{
	_number = number;
	_window = window;
	_scene = scene;
	_level = level;
	_viewport = window->addViewport();
	_camera.setNew(_scene);
	_camera->setMaxViewSize(320.0f);
	_viewport->setScene(_scene);
	_viewport->setCamera(_camera->getSceneCamera());
	_speed = 1.0f;
	_maxSpeed = 10.0f;
}

Player::~Player()
{
	_level->removeObject(_character);
	if(_window.isValid())
	{
		_window->removeWidget(_viewport);
	}
}

bool Player::hasCharacter() const
{
	return _character.isValid();
}

void Player::setCharacter(std::string const & character)
{
	if(_character.isValid())
	{
		_level->removeObject(_character);
	}
	_character.setNew(_scene, "data/bullet.png", Recti::minMax(0, 0, 16, 16));
	_character->setPosition(Vector2f(math::random(0.0f, 1.0f), math::random(0.0f, 1.0f)));
	_character->setSolid(true);
	_level->addObject(_character);
}

Ptr<gui::Viewport> Player::getViewport ()
{
	return _viewport;
}

void Player::handleEvent(Event const & event)
{
	if(!_character.isValid())
	{
		return;
	}
	if(_number == 1)
	{
		if(event.type == Event::Keyboard)
		{
			KeyboardEvent const & ke = event.as<KeyboardEvent>();
			if(ke.key == KeyboardEvent::A)
			{
				_moving[0] -= ke.pressed ? 1 : -1;
			}
			if(ke.key == KeyboardEvent::S)
			{
				_moving[1] -= ke.pressed ? 1 : -1;
			}
			if(ke.key == KeyboardEvent::D)
			{
				_moving[0] += ke.pressed ? 1 : -1;
			}
			if(ke.key == KeyboardEvent::W)
			{
				_moving[1] += ke.pressed ? 1 : -1;
			}
		}
	}
	if(event.type == Event::Update)
	{
		Vector2f velocity = _character->getVelocity();
		velocity += Vector2f(_moving) * _speed;
		if(velocity.normSq() > _maxSpeed * _maxSpeed)
		{
			velocity = velocity.unit() * _maxSpeed;
		}
		_character->setVelocity(velocity);
	}
	else if(event.type == Event::PreRenderUpdate)
	{
		_camera->setPosition(_character->getPosition());
	}
}

