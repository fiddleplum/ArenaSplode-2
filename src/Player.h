#pragma once

#include <kit/gui/viewport.h>
#include <kit/flat/camera.h>
#include <kit/flat/object.h>
#include <kit/window.h>

using namespace kit;

class Player : public kit::Noncopyable
{
public:
	Player (Ptr<Window> window, Ptr<Scene> scene);

	~Player();

	Ptr<gui::Viewport> getViewport ();

private:
	Ptr<Window> _window;
	Ptr<Scene> _scene;
	Ptr<gui::Viewport> _viewport;
	OwnPtr<flat::Camera> _camera;
	OwnPtr<flat::Object> _character;
};

