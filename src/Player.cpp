#include "Player.h"
#include <kit/gui/viewport.h>

Player::Player (Ptr<Window> window, Ptr<scene::Scene> scene, Ptr<Level> level)
{
	_window = window;
	_scene = scene;
	_level = level;
	_viewport = window->addViewport();
	_camera.set(new flat::Camera(_scene));
	_camera->setMaxViewSize(320.0f);
	_viewport->setScene(_scene);
	_viewport->setCamera(_camera->getSceneCamera());
	_character.set(new Object(_scene, "data/bullet.png", Recti::minMax(0, 0, 16, 16)));
	_character->setPosition(Vector2f(0, -60.f));
	_level->addObject(_character);
	_speed = 1.0f;
	_maxSpeed = 10.0f;
}

Player::~Player()
{
	if(_level.isValid())
	{
		_level->removeObject(_character);
	}
	if(_window.isValid())
	{
		_window->removeWidget(_viewport);
	}
}

Ptr<gui::Viewport> Player::getViewport ()
{
	return _viewport;
}

void Player::handleEvent(Event const & event)
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
	else if(event.type == Event::Update)
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

