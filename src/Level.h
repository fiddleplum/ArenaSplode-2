#pragma once

#include "Object.h"
#include <kit/scene/scene.h>
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
	Level(Ptr<scene::Scene> scene, Vector2i size);

	void addObject(OwnPtr<Object> object);

	void removeObject(Ptr<Object> object);

	void update();

private:
	Ptr<scene::Scene> _scene;
	Vector2i _tileSize;
	Vector2i _size;
	std::vector<Tile> _tiles;
	PtrSet<Object> _objects;
};

