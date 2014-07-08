#pragma once

#include <kit/scene.h>
#include <kit/flat/object.h>

using namespace kit;

class Tile
{
public:
	enum {Floor, Wall, NumTypes};
	OwnPtr<flat::Object> sprite;
	int type;
};

class Level
{
public:
	Level(Ptr<Scene> scene, Vector2i size);

private:
	Ptr<Scene> _scene;
	Vector2i _size;
	std::vector<Tile> _tiles;
};

