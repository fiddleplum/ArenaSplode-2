#pragma once

#include <kit/window.h>

class CharacterMenu
{
public:
	CharacterMenu(int controller, Ptr<kit::Window> window);
	~CharacterMenu();
	void handleEvent(kit::Event const & event);
	void updateWidgets(Recti bounds);
	void setCharacterChosenFunction(std::function<void(std::string const & filename)> function);

private:
	void moveSelection(int offset);
	void chooseSelection();

	Ptr<kit::Window> window;
	int controller;
	bool controllerCentered;
	int currentCharacter;
	Recti bounds;
	std::function<void(std::string const & filename)> characterChosenFunction;
	std::vector<std::string> filenames;
	std::vector<Ptr<kit::gui::Sprite>> characterSprites;
	Ptr<kit::gui::Sprite> selectionSprite;

	float stepAngle;
	float currentAngle;
	float targetAngle;
};

