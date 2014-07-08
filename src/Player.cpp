#include "Player.h"
#include <kit/gui/viewport.h>

Player::Player (Ptr<Window> window, Ptr<Scene> scene)
{
	_window = window;
	_scene = scene;
	_viewport = window->addViewport();
	_camera.set(new flat::Camera(_scene));
	_camera->setMaxViewSize(320.0f);
	_viewport->setScene(_scene);
	_viewport->setCamera(_camera->getSceneCamera());
	_character.set(new flat::Object(_scene));
	_character->setTexture("data/bullet.png");
	_character->setTextureCoords(Recti::minMax(0, 0, 16, 16));
	_character->setZ(1);
}

Player::~Player()
{
	_character->setZ(4);
	if(_window.isValid())
	{
		_window->removeWidget(_viewport);
	}
}

Ptr<gui::Viewport> Player::getViewport ()
{
	return _viewport;
}

