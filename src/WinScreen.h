#pragma once

#include <kit/Window.h>

class WinScreen
{
public:
	WinScreen(Ptr<kit::Window> window, std::string const & characterFilename);

	~WinScreen();

	void handleEvent(kit::Event const & event);

private:
	Ptr<kit::Window> window;
	Ptr<kit::gui::Sprite> sprite;
	float timeShownSoFar;
};