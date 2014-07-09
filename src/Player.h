#pragma once

#include "Level.h"
#include <kit/gui/viewport.h>
#include <kit/flat/camera.h>
#include <kit/flat/object.h>
#include <kit/window.h>

using namespace kit;

class Player : public kit::Noncopyable
{
public:
	Player (int number, Ptr<Window> window, Ptr<scene::Scene> scene, Ptr<Level> level);

	~Player();

	Ptr<gui::Viewport> getViewport ();

	void handleEvent(Event const & event);

private:
	Ptr<Window> _window;
	Ptr<scene::Scene> _scene;
	Ptr<Level> _level;
	Ptr<gui::Viewport> _viewport;
	OwnPtr<flat::Camera> _camera;
	OwnPtr<Object> _character;

	int _number;
	Vector2i _moving;
	float _speed;
	float _maxSpeed;
};

