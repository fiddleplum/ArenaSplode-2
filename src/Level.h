#pragma once

#include "Object.h"
#include <kit/scene/scene.h>
#include <kit/flat/object.h>

class Tile
{
public:
	enum {Floor, Wall, NumTypes};
	OwnPtr<kit::flat::Object> sprite;
	int type;
};

class Level
{
public:
	Level(Ptr<kit::scene::Scene> scene, Vector2i size);

	void addObject(OwnPtr<Object> object);

	void removeObject(Ptr<Object> object);

	void setPaused(bool paused);

	void update();

	std::pair<Ptr<Object>, float> getNearestObject(Ptr<Object> reference);

private:
	void updatePatch(Vector2i tile);

	Ptr<kit::scene::Scene> scene;
	bool paused;
	Vector2i tileSize;
	Vector2i size;
	std::vector<Tile> tiles;
	std::vector<OwnPtr<kit::flat::Object>> patchSprites;
	PtrSet<Object> objects;
};

