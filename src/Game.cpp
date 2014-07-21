#include "Game.h"
#include "Player.h"
#include "Level.h"
#include "NumPlayersMenu.h"
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
button.setNew(window); // adds to window
...
button.setNull(); // removes from window
This way, RAII, the button appears the moment it is created. No more add/remove calls from the user. It just works.

Namespaces:
Make kit have no namespace. Make all app things be in the app namespace. ??? Maybe
The reason is that it is annoying to have to say kit::Ptr, and having "using namespace kit" in headers is a bad idea.

Fix Bug:
Sometimes, an item or the character gets rendered first and then the tiles are rendered on top. The transparent pixels
of the items still modify the depth buffer, so that when the tile is rendered, it doesn't render on the tranparent pixels,
so there is a black area. Figure out how to not render pixels if they are transparent.

Fix Bug:
Fix game loop framerate to be fixed rate, but handle super fast framerates.
*/

OwnPtr<Game> game;

Game::Game()
{
	window = kit::app::addWindow("ArenaSplode 2");
	window->setHandleContainerEventFunction(std::bind(&Game::handleEvent, this, std::placeholders::_1));
	window->setUpdateWidgetBoundsFunction(std::bind(&Game::updateWidgets, this));

	numPlayersMenu.setNew(window);
	numPlayersMenu->setPlayersButtonPressedFunction(std::bind(&Game::numPlayersChosen, this, std::placeholders::_1));
}

Game::~Game()
{
	numPlayersMenu.setNull();
	players.clear();
	level.setNull();
}

void Game::handleEvent(kit::Event const & event)
{
	if(numPlayersMenu.isValid())
	{
		numPlayersMenu->handleEvent(event);
	}
	for(auto player : players)
	{
		player->handleEvent(event);
	}
}

void Game::handleSceneEvent(kit::Event const & event)
{
	for(auto player : players)
	{
		player->handleSceneEvent(event);
	}
	if(event.type == kit::Event::Update)
	{
		static float lastTime = 0;
		float thisTime = kit::app::getTime();
		kit::log::write(std::to_string(1.0f / (thisTime - lastTime)));
		lastTime = thisTime;
		if(level.isValid())
		{
			level->update();
		}
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
		kit::audio::play("sound/smw_power-up.wav");
		level->setPaused(false);
	}
}

void Game::numPlayersChosen(int numPlayers)
{
	numPlayersMenu.setNull();
	scene = kit::app::addScene();
	scene->setEventHandler(std::bind(&Game::handleSceneEvent, this, std::placeholders::_1));
	level.setNew(scene, Vector2i(20, 20));
	for(int i = 0; i < numPlayers; i++)
	{
		players.push_back(OwnPtr<Player>::createNew(i, window, scene, level));
		players.back()->setCharacterChosenFunction(std::bind(&Game::characterChosen, this, std::placeholders::_1));
	}

	updateWidgets();

	state = CharacterSelection;
}

void kit::start(std::vector<std::string> const &)
{
	game.setNew();
}

void kit::finish()
{
	game.setNull();
}

