#include "Level.h"

Level::Level(Ptr<Scene> scene, Vector2i size)
{
	_scene = scene;
	_size = size;

	for(int y = 0; y < size[1]; y++)
	{
		for(int x = 0; x < size[0]; x++)
		{
			Tile tile;
			tile.type = rand() % Tile::NumTypes;
			tile.sprite.set(new flat::Object(_scene));
			tile.sprite->setTexture("data/tiles.png");
			tile.sprite->setTextureCoords(Recti::minSize(64 * tile.type, 0, 64, 64));
			tile.sprite->setPosition(Vector2i(x * 64, y * 64));
			_tiles.push_back(tile);
		}
	}
}

