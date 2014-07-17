#pragma once

#include <kit/window.h>
#include <kit/file_system.h>
#include <kit/math_util.h>

class PlayerSelection
{
public:
	PlayerSelection(Ptr<Player> player, Ptr<Window> window)
	{
		_player = player;
		_window = window;

		_currentCharacter = 0;
		targetAngle = 0;
		currentAngle = 0;

		// Get character images;
		_filenames = fs::getFilesInFolder("art/characters");

		for(auto filename : _filenames)
		{
			auto sprite = window->addSprite();
			sprite->setTexture("art/characters/" + filename);
			sprite->setTextureBounds(Recti::minSize(0, 0, 64, 64));
			_characterSprites.push_back(sprite);
		}

		stepAngle = (float)1.f / (float)_characterSprites.size() * 2.0f * 3.14159f;

		selectionSprite = window->addSprite();
		selectionSprite->setTexture("art/selection_box.png");
		selectionSprite->setTextureBounds(Recti::minSize(0, 0, 64, 64));

		updateButtons();
	}

	~PlayerSelection()
	{
		if(!_window.isValid())
		{
			return;
		}
	}

	void handleEvent(Event const & event)
	{
		if(event.type == Event::Keyboard)
		{
			auto ke = event.as<KeyboardEvent>();
			if(ke.pressed)
			{
				if(ke.key == KeyboardEvent::Left)
				{
					_currentCharacter--;
					if(_currentCharacter < 0)
					{
						_currentCharacter += _characterSprites.size();
					}
					targetAngle = _currentCharacter * stepAngle;
				}
				if(ke.key == KeyboardEvent::Right)
				{
					_currentCharacter++;
					if(_currentCharacter >= (int)_characterSprites.size())
					{
						_currentCharacter -= _characterSprites.size();
					}
					targetAngle = _currentCharacter * stepAngle;
				}
			}
		}
		else if(event.type == Event::Update)
		{
			UpdateEvent ue = event.as<UpdateEvent>();
			while(targetAngle - currentAngle > 3.14159f)
			{
				targetAngle -= 2.f * 3.14159f;
			}
			while(targetAngle - currentAngle < -3.14159f)
			{
				targetAngle += 2.f * 3.14159f;
			}
			currentAngle += (targetAngle - currentAngle) * 5.f * ue.dt;

			Recti viewportBounds = _player->getViewport()->getBounds();
			float radius = (float)(20 * _characterSprites.size());
			_window->setWidgetPlacement(selectionSprite, Vector2f(0, 0), Vector2f(.5f, .5f), (viewportBounds.min + viewportBounds.max) / 2);
			Vector2i center = (viewportBounds.min + viewportBounds.max) / 2 + Vector2i(0, (int)radius);
			for(unsigned int count = 0; count < _characterSprites.size(); count++)
			{
				int i = (count + _currentCharacter) % _characterSprites.size();
				float angle = (float)i * stepAngle - currentAngle;
				float thisRadius = radius;
				if(i == _currentCharacter)
				{
					angle += math::random(-.05f, +.05f);
					thisRadius += math::random(-5.f, +5.f);
				}

				_window->setWidgetPlacement(_characterSprites[i], Vector2f(0, 0), Vector2f(.5f, .5f), Vector2i(center[0] + (int)(thisRadius * sin(angle)), center[1] - (int)(thisRadius * cos(angle))));
			}
		}
	}

	void updateButtons()
	{
		Recti viewportBounds = _player->getViewport()->getBounds();
		float radius = (float)(20 * _characterSprites.size());
		_window->setWidgetPlacement(selectionSprite, Vector2f(0, 0), Vector2f(.5f, .5f), (viewportBounds.min + viewportBounds.max) / 2);
		Vector2i center = (viewportBounds.min + viewportBounds.max) / 2 + Vector2i(0, (int)radius);
		for(unsigned int count = 0; count < _characterSprites.size(); count++)
		{
			int i = (count + _currentCharacter) % _characterSprites.size();
			float angle = (float)count / (float)_characterSprites.size() * 2.0f * 3.14159f;
			_window->setWidgetPlacement(_characterSprites[i], Vector2f(0, 0), Vector2f(.5f, .5f), Vector2i(center[0] + (int)(radius * sin(angle)), center[1] - (int)(radius * cos(angle))));
		}
	}

	void setCharacterChosenFunction(std::function<void(int i, std::string const & filename)> function)
	{
		_characterChosenFunction = function;
	}

private:
	Ptr<Window> _window;
	Ptr<Player> _player;
	int _currentCharacter;
	std::function<void(int i, std::string const & filename)> _characterChosenFunction;
	std::vector<std::string> _filenames;
	std::vector<Ptr<gui::Sprite>> _characterSprites;
	Ptr<gui::Sprite> selectionSprite;

	float stepAngle;
	float currentAngle;
	float targetAngle;
};

