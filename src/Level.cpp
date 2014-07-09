#include "Level.h"
#include <kit/math_util.h>

Level::Level(Ptr<scene::Scene> scene, Vector2i size)
{
	_scene = scene;
	_size = size;
	_tileSize.set(64, 64);

	for(int y = 0; y < size[1]; y++)
	{
		for(int x = 0; x < size[0]; x++)
		{
			Tile tile;
			tile.type = rand() % Tile::NumTypes;
			tile.sprite.set(new flat::Object(_scene));
			tile.sprite->setTexture("data/tiles.png");
			tile.sprite->setTextureCoords(Recti::minSize(_tileSize[0] * tile.type, 0, _tileSize[0], _tileSize[1]));
			tile.sprite->setPosition(Vector2i(x * _tileSize[0], y * _tileSize[1]));
			_tiles.push_back(tile);
		}
	}
}

void Level::addObject(OwnPtr<Object> object)
{
	_objects.insert(object);
}

void Level::removeObject(Ptr<Object> object)
{
	_objects.erase(object);
}

void Level::update()
{
	for(auto object : _objects)
	{
		object->doPhysics(1.f / 30.f);
	}
	for(int y = 0; y < _size[1]; y++)
	{
		for(int x = 0; x < _size[0]; x++)
		{
			Tile const & tile = _tiles[y * _size[0] + x];
			if(tile.type == Tile::Floor)
			{
				continue;
			}
			Rectf bounds = Rectf::minSize((float)(x * _tileSize[0]), (float)(y * _tileSize[1]), (float)_tileSize[0], (float)_tileSize[1]);
			for(auto object : _objects)
			{
				// Get closest point to circle within tile
				Vector2f closest = bounds.closest(object->getPosition());

				if((closest - object->getPosition()).normSq() < object->getRadius() * object->getRadius())
				{
					Vector2f r = object->getPosition() - closest;
					if(!r.isZero())
					{
						object->setPosition(object->getPosition() + r.unit() * (object->getRadius() - r.norm()));
					}
				}
			}
		}
	}
}

