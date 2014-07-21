#pragma once

#include <kit/ptr.h>
#include <kit/window.h>
#include <kit/scene/scene.h>
#include <vector>

class NumPlayersMenu;
class Player;
class Level;

class Game
{
public:
	Game();
	~Game();

	void handleEvent(kit::Event const & event);

	void handleSceneEvent(kit::Event const & event);

	void updateWidgets();

	void characterChosen(int player);

	void numPlayersChosen(int numPlayers);

	Ptr<kit::Window> window;
	Ptr<kit::scene::Scene> scene;

	enum State { NumPlayersSelection, CharacterSelection, Playing, Win };
	State state;

	OwnPtr<NumPlayersMenu> numPlayersMenu;
	std::vector<OwnPtr<Player>> players;
	OwnPtr<Level> level;
};

extern OwnPtr<Game> game;