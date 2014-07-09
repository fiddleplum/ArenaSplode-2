#include "Player.h"
#include "Level.h"
#include <kit/start_finish.h>
#include <kit/window.h>
#include <kit/gui/button.h>
#include <kit/resources.h>
#include <kit/scene/scene.h>
#include <kit/log.h>
#include <kit/audio.h>

using namespace kit;

Ptr<Window> window;
Ptr<gui::Button> startButton;
Ptr<scene::Scene> _scene;
std::vector<OwnPtr<Player>> players;
OwnPtr<Level> level;

void handleWindowEvent (Event const & event)
{
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

void updateMenu ()
{
	if(startButton.isValid())
	{
		window->setWidgetPlacement(startButton, Vector2f(.5f, .5f), Vector2f(.5f, .5f), Vector2i(0, 0));
	}
	if(players.size() == 1)
	{
		window->setWidgetPlacement(players[0]->getViewport(), Vector2f(0, 0), Vector2f(0, 0), Vector2f(0, 0));
		window->setWidgetPlacementSize(players[0]->getViewport(), Vector2f(1, 1), Vector2f(0, 0));
	}
	else if (players.size() == 2)
	{
		window->setWidgetPlacement(players[0]->getViewport(), Vector2f(0, 0), Vector2f(0, 0), Vector2f(0, 0));
		window->setWidgetPlacementSize(players[0]->getViewport(), Vector2f(.5f, 1), Vector2f(0, 0));
		window->setWidgetPlacement(players[1]->getViewport(), Vector2f(.5f, 0), Vector2f(0, 0), Vector2f(0, 0));
		window->setWidgetPlacementSize(players[1]->getViewport(), Vector2f(.5f, 1), Vector2f(0, 0));
	}
	else if (players.size() == 3)
	{
		window->setWidgetPlacement(players[0]->getViewport(), Vector2f(0, 0), Vector2f(0, 0), Vector2f(0, 0));
		window->setWidgetPlacementSize(players[0]->getViewport(), Vector2f(.333f, 1), Vector2f(0, 0));
		window->setWidgetPlacement(players[1]->getViewport(), Vector2f(.333f, 0), Vector2f(0, 0), Vector2f(0, 0));
		window->setWidgetPlacementSize(players[1]->getViewport(), Vector2f(.333f, 1), Vector2f(0, 0));
		window->setWidgetPlacement(players[2]->getViewport(), Vector2f(.667f, 0), Vector2f(0, 0), Vector2f(0, 0));
		window->setWidgetPlacementSize(players[2]->getViewport(), Vector2f(.333f, 1), Vector2f(0, 0));
	}
	else if (players.size() == 4)
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


void startButtonPressed()
{
	audio::play("data/go.ogg");
	window->removeWidget(startButton);

	_scene = app::addScene();
	_scene->setEventHandler(handleSceneEvent);
	level.set(new Level(_scene, Vector2i(10, 10)));
	players.push_back(OwnPtr<Player>(new Player(window, _scene, level)));
}

void kit::start (std::vector<std::string> const &)
{
	window = app::addWindow("ArenaSplode 2");
	window->setHandleContainerEventFunction(handleWindowEvent);
	window->setUpdateWidgetBoundsFunction(updateMenu);

	startButton = window->addButton();
	startButton->setTexture(resources::getTextureFromFile("art/start_button.png"));
	startButton->setTextureBounds(Recti::minSize(0, 0, 256, 128));
	startButton->setPressFunction(startButtonPressed);
}

void kit::finish ()
{
}

