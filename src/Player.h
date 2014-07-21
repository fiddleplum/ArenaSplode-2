#pragma once

#include "character.h"
#include <kit/gui/viewport.h>
#include <kit/flat/camera.h>
#include <kit/window.h>

class Level;
class Object;
class CharacterMenu;

class Player : public Noncopyable
{
public:
	Player(int number, Ptr<kit::Window> window, Ptr<kit::scene::Scene> scene, Ptr<Level> level);

	~Player();

	int getNumber() { return number; }

	bool hasCharacter() const;

	void setCharacter(std::string const & character);

	void handleEvent(kit::Event const & event);

	void handleSceneEvent(kit::Event const & event);

	void updateWidgets();

	void setCharacterChosenFunction(std::function<void(int number)> function);

private:
	void characterChosen(std::string const & characterFilename);

	Ptr<kit::Window> window;
	Ptr<kit::scene::Scene> scene;
	Recti bounds;
	Ptr<Level> level;
	Ptr<kit::gui::Viewport> viewport;
	OwnPtr<kit::flat::Camera> camera;
	OwnPtr<Character> character;
	OwnPtr<CharacterMenu> characterMenu;
	std::function<void(int number)> characterChosenFunction;

	int number;
	Vector2i moving;
	bool looking;
	Vector2f lookDirection;
	float speed;
};

