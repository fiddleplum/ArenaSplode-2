#pragma once

#include "Object.h"
#include <kit/scene/scene.h>
#include <kit/flat/object.h>
#include <kit/set.h>

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

	Ptr<kit::scene::Scene> getScene() const;

	void addObject(OwnPtr<Object> object);

	void removeObject(Object const * object);

	Tile & getTile(Vector2i position);
	Tile const & getTile(Vector2i position) const;

	Vector2i getTileSize() const { return tileSize; }

	Vector2i getSize() const { return size; }

	void setPaused(bool paused);

	void update(float dt);

	void preRenderUpdate();

	std::vector<std::pair<Ptr<Object>, float>> getObjectsWithinCircle(Vector2f position, float radius, Object const * exclude) const;

	std::pair<Ptr<Object>, float> getNearestObject(Vector2f position, Object const * exclude) const;

private:
	void updatePatch(Vector2i tile);

	Ptr<kit::scene::Scene> scene;
	bool paused;
	Vector2i tileSize;
	Vector2i size;
	std::vector<Tile> tiles;
	std::vector<OwnPtr<kit::flat::Object>> patchSprites;
	kit::Set<OwnPtr<Object>> objects;
};

