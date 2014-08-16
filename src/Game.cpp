#include "Game.h"
#include "Player.h"
#include "Level.h"
#include "NumPlayersMenu.h"
#include "WinScreen.h"
#include <kit/start_finish.h>
#include <kit/log.h>
#include <kit/audio.h>
#include <kit/list.h>

/*

New Idea about events:
Instead of calling handleEvent(Event) on each widget for each event, have the app process all events at top level,
and then put them in an kit::input namespace. In the game loop, the app would call handleEvents() on each widget.
Each widget then would call something like kit:input::getMouseMovement(), which would get the mouse movement since
the last frame. There would be mouseMovedOffset(), mouseButtonWasPressed(int), mouseButtonWasUnpressed(int),etc.

Also, it really should be:
OwnPtr<Button> button;
button.create(window); // adds to window
...
button.destroy(); // removes from window
This way, RAII, the button appears the moment it is created. No more add/remove calls from the user. It just works.

Namespaces:
Make kit have no namespace. Make all app things be in the app namespace. ??? Maybe
The reason is that it is annoying to have to say kit::Ptr, and having "using namespace kit" in headers is a bad idea.

Fix Bug:
Fix game loop framerate to be fixed rate, but handle super fast framerates.
*/

OwnPtr<Game> game;

Game::Game()
{
	window = kit::app::addWindow("ArenaSplode 2");
	window->setHandleContainerEventFunction(std::bind(&Game::handleEvent, this, std::placeholders::_1));
	window->setUpdateWidgetBoundsFunction(std::bind(&Game::updateWidgets, this));

	numPlayersMenu.create(window);
	numPlayersMenu->setPlayersButtonPressedFunction(std::bind(&Game::numPlayersChosen, this, std::placeholders::_1));
	state = NumPlayersSelection;
	willRestart = false;
}

Game::~Game()
{
	numPlayersMenu.destroy();
	players.clear();
	level.destroy();
}

void Game::handleEvent(kit::Event const & event)
{
	if(willRestart)
	{
		winScreen.destroy();
		level.destroy();
		kit::app::removeScene(scene);
		players.clear();
		numPlayersMenu.create(window);
		numPlayersMenu->setPlayersButtonPressedFunction(std::bind(&Game::numPlayersChosen, this, std::placeholders::_1));
		numPlayersMenu->updateWidgets();
		state = NumPlayersSelection;
		willRestart = false;
		return;
	}
	if(numPlayersMenu.isValid())
	{
		numPlayersMenu->handleEvent(event);
	}
	if(winScreen.isValid())
	{
		winScreen->handleEvent(event);
	}
	for(auto player : players)
	{
		player->handleEvent(event);
		if(players.empty())
		{
			break;
		}
	}
}

void Game::handleSceneEvent(kit::Event const & event)
{
	for(auto player : players)
	{
		player->handleSceneEvent(event);
		if(players.empty())
		{
			break;
		}
	}
	if(event.type == kit::Event::Update)
	{
		static float lastTime = 0;
		float thisTime = kit::app::getTime();
		//kit::log::write(std::to_string(1.0f / (thisTime - lastTime)));
		lastTime = thisTime;
		if(level.isValid())
		{
			level->update(event.as<kit::UpdateEvent>().dt);
		}
	}
	else if(event.type == kit::Event::PreRenderUpdate)
	{
		level->preRenderUpdate();
	}
}

void Game::updateWidgets()
{
	if(numPlayersMenu.isValid())
	{
		numPlayersMenu->updateWidgets();
	}
	for(auto player : players)
	{
		player->updateWidgets();
	}
}

void Game::characterChosen(int)
{
	bool allDoneChoosing = true;
	for(auto player : players)
	{
		if(!player->hasCharacter())
		{
			allDoneChoosing = false;
		}
	}
	if(allDoneChoosing)
	{
		kit::audio::play("sounds/smw_power-up.wav");
		level->setPaused(false);
	}
}

void Game::numPlayersChosen(int numPlayers)
{
	numPlayersMenu.destroy();
	scene = kit::app::addScene();
	scene->setEventHandler(std::bind(&Game::handleSceneEvent, this, std::placeholders::_1));
	level.create(scene, Vector2i(25, 25));
	for(int i = 0; i < numPlayers; i++)
	{
		OwnPtr<Player> player;
		player.create(i, window, scene, level);
		players.push_back(player);
		players.back()->setCharacterChosenFunction(std::bind(&Game::characterChosen, this, std::placeholders::_1));
	}

	updateWidgets();

	state = CharacterSelection;
}

void Game::playerWins(int player)
{
	winScreen.create(window, players[player]->getCharacterFilename());
}

void Game::restart()
{
	willRestart = true;
}

void kit::start(std::vector<std::string> const &)
{
	game.create();
}

void kit::finish()
{
	game.destroy();
}

