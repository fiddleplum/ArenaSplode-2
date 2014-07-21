#include "CharacterMenu.h"
#include <kit/file_system.h>
#include <kit/math_util.h>

CharacterMenu::CharacterMenu(Ptr<kit::Window> _window)
{
	window = _window;

	currentCharacter = 0;
	targetAngle = 0;
	currentAngle = 0;

	// Get character images;
	filenames = kit::fs::getFilesInFolder("art/characters");

	for(auto filename : filenames)
	{
		auto sprite = window->addSprite();
		sprite->setTexture("art/characters/" + filename);
		sprite->setTextureBounds(Recti::minSize(0, 0, 64, 64));
		characterSprites.push_back(sprite);
	}

	stepAngle = (float)1.f / (float)characterSprites.size() * 2.0f * 3.14159f;

	selectionSprite = window->addSprite();
	selectionSprite->setTexture("art/selection_box.png");
	selectionSprite->setTextureBounds(Recti::minSize(0, 0, 64, 64));
}

CharacterMenu::~CharacterMenu()
{
	if(!window.isValid())
	{
		return;
	}
	window->removeWidget(selectionSprite);
	for(auto sprite : characterSprites)
	{
		window->removeWidget(sprite);
	}
}

void CharacterMenu::handleEvent(kit::Event const & event)
{
	if(event.type == kit::Event::Keyboard)
	{
		auto ke = event.as<kit::KeyboardEvent>();
		if(ke.pressed)
		{
			if(ke.key == kit::KeyboardEvent::Left)
			{
				currentCharacter--;
				if(currentCharacter < 0)
				{
					currentCharacter += characterSprites.size();
				}
				targetAngle = currentCharacter * stepAngle;
			}
			else if(ke.key == kit::KeyboardEvent::Right)
			{
				currentCharacter++;
				if(currentCharacter >= (int)characterSprites.size())
				{
					currentCharacter -= characterSprites.size();
				}
				targetAngle = currentCharacter * stepAngle;
			}
			else if(ke.key == kit::KeyboardEvent::Enter)
			{
				if(characterChosenFunction)
				{
					characterChosenFunction(filenames[currentCharacter]);
				}
			}
		}
	}
	else if(event.type == kit::Event::Update)
	{
		kit::UpdateEvent ue = event.as<kit::UpdateEvent>();
		while(targetAngle - currentAngle > 3.14159f)
		{
			targetAngle -= 2.f * 3.14159f;
		}
		while(targetAngle - currentAngle < -3.14159f)
		{
			targetAngle += 2.f * 3.14159f;
		}
		currentAngle += (targetAngle - currentAngle) * 5.f * ue.dt;

		float radius = (float)(20 * characterSprites.size());
		window->setWidgetPlacement(selectionSprite, Vector2f(0, 0), Vector2f(.5f, .5f), (bounds.min + bounds.max) / 2);
		Vector2i center = (bounds.min + bounds.max) / 2 + Vector2i(0, (int)radius);
		for(unsigned int count = 0; count < characterSprites.size(); count++)
		{
			int i = (count + currentCharacter) % characterSprites.size();
			float angle = (float)i * stepAngle - currentAngle;
			float thisRadius = radius;
			if(i == currentCharacter)
			{
				angle += kit::math::random(-.05f, +.05f);
				thisRadius += kit::math::random(-5.f, +5.f);
			}

			window->setWidgetPlacement(characterSprites[i], Vector2f(0, 0), Vector2f(.5f, .5f), Vector2i(center[0] + (int)(thisRadius * sin(angle)), center[1] - (int)(thisRadius * cos(angle))));
		}
	}
}

void CharacterMenu::updateWidgets(Recti _bounds)
{
	bounds = _bounds;
}

void CharacterMenu::setCharacterChosenFunction(std::function<void(std::string const & filename)> function)
{
	characterChosenFunction = function;
}

