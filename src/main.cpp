#include "Player.h"
#include "Level.h"
#include "StartMenu.h"
#include "PlayerSelection.h"
#include <kit/start_finish.h>
#include <kit/window.h>
#include <kit/gui/button.h>
#include <kit/resources.h>
#include <kit/scene/scene.h>
#include <kit/log.h>
#include <kit/audio.h>

#include <kit/open_gl.h>

using namespace kit;

Ptr<Window> window;
Ptr<scene::Scene> _scene;
std::vector<OwnPtr<Player>> players;
OwnPtr<Level> level;
OwnPtr<StartMenu> startMenu;
std::vector<OwnPtr<PlayerSelection>> playerSelections;

class Game
{
public:
private:
};

void handleWindowEvent(Event const & event)
{
	for(auto playerSelection : playerSelections)
	{
		playerSelection->handleEvent(event);
	}
	if(event.type == Event::Keyboard)
	{
		auto ke = event.as<KeyboardEvent>();
		if(ke.pressed)
		{
			if(ke.key == KeyboardEvent::A)
			{
			}
		}
	}
}

void updateWindowBounds()
{
	if(startMenu.isValid())
	{
		startMenu->update();
	}
	if(players.size() == 1)
	{
		window->setWidgetPlacement(players[0]->getViewport(), Vector2f(0, 0), Vector2f(0, 0), Vector2f(0, 0));
		window->setWidgetPlacementSize(players[0]->getViewport(), Vector2f(1, 1), Vector2f(0, 0));
	}
	else if(players.size() == 2)
	{
		window->setWidgetPlacement(players[0]->getViewport(), Vector2f(0, 0), Vector2f(0, 0), Vector2f(0, 0));
		window->setWidgetPlacementSize(players[0]->getViewport(), Vector2f(.5f, 1), Vector2f(0, 0));
		window->setWidgetPlacement(players[1]->getViewport(), Vector2f(.5f, 0), Vector2f(0, 0), Vector2f(0, 0));
		window->setWidgetPlacementSize(players[1]->getViewport(), Vector2f(.5f, 1), Vector2f(0, 0));
	}
	else if(players.size() == 3)
	{
		window->setWidgetPlacement(players[0]->getViewport(), Vector2f(0, 0), Vector2f(0, 0), Vector2f(0, 0));
		window->setWidgetPlacementSize(players[0]->getViewport(), Vector2f(.333f, 1), Vector2f(0, 0));
		window->setWidgetPlacement(players[1]->getViewport(), Vector2f(.333f, 0), Vector2f(0, 0), Vector2f(0, 0));
		window->setWidgetPlacementSize(players[1]->getViewport(), Vector2f(.333f, 1), Vector2f(0, 0));
		window->setWidgetPlacement(players[2]->getViewport(), Vector2f(.667f, 0), Vector2f(0, 0), Vector2f(0, 0));
		window->setWidgetPlacementSize(players[2]->getViewport(), Vector2f(.333f, 1), Vector2f(0, 0));
	}
	else if(players.size() == 4)
	{
		window->setWidgetPlacement(players[0]->getViewport(), Vector2f(0, 0), Vector2f(0, 0), Vector2f(0, 0));
		window->setWidgetPlacementSize(players[0]->getViewport(), Vector2f(.5f, .5f), Vector2f(0, 0));
		window->setWidgetPlacement(players[1]->getViewport(), Vector2f(.5f, 0), Vector2f(0, 0), Vector2f(0, 0));
		window->setWidgetPlacementSize(players[1]->getViewport(), Vector2f(.5f, .5f), Vector2f(0, 0));
		window->setWidgetPlacement(players[2]->getViewport(), Vector2f(0, .5f), Vector2f(0, 0), Vector2f(0, 0));
		window->setWidgetPlacementSize(players[2]->getViewport(), Vector2f(.5f, .5f), Vector2f(0, 0));
		window->setWidgetPlacement(players[3]->getViewport(), Vector2f(.5f, .5f), Vector2f(0, 0), Vector2f(0, 0));
		window->setWidgetPlacementSize(players[3]->getViewport(), Vector2f(.5f, .5f), Vector2f(0, 0));
	}
	for(auto playerSelection : playerSelections)
	{
		playerSelection->updateButtons();
	}
}

void handleSceneEvent(Event const & event)
{
	if(event.type == Event::Update)
	{
		level->update();
	}
	for(auto player : players)
	{
		player->handleEvent(event);
	}
}

void characterChosen(int i, std::string const & filename)
{
	players[i - 1]->setCharacter(filename);
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
		playerSelections.clear();
		audio::play("sound/smw_power-up.wav");
		level->setPaused(false);
	}
	else
	{
		audio::play("sound/smw_coin.wav");
	}
}

void playersButtonPressed(int numPlayers)
{
	startMenu.setNull();

	_scene = app::addScene();
	_scene->setEventHandler(handleSceneEvent);
	level.setNew(_scene, Vector2i(10, 10));
	for(int i = 1; i <= numPlayers; i++)
	{
		players.push_back(OwnPtr<Player>::createNew(i, window, _scene, level));
	}

	for(int i = 1; i <= numPlayers; i++)
	{
		playerSelections.push_back(OwnPtr<PlayerSelection>::createNew(players[i - 1], window));
		playerSelections[i - 1]->setCharacterChosenFunction(characterChosen);
	}

	updateWindowBounds();
}

void kit::start(std::vector<std::string> const &)
{
	window = app::addWindow("ArenaSplode 2");
	window->setHandleContainerEventFunction(handleWindowEvent);
	window->setUpdateWidgetBoundsFunction(updateWindowBounds);
	startMenu.setNew(window);
	startMenu->setPlayersButtonPressedFunction(playersButtonPressed);
}

void kit::finish()
{
	startMenu.setNull();
	players.clear();
	level.setNull();
}

