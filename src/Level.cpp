#include "Level.h"
#include "Sword.h"
#include "Shrinker.h"
#include "ChainWand.h"
#include "Chain.h"
#include "Nuke.h"
#include "Crazy.h"
#include "RocketLauncher.h"
#include "Shell.h"
#include <kit/audio.h>
#include <kit/math_util.h>
#include "Game.h"

int patchSize = 16;

Level::Level(Ptr<kit::scene::Scene> _scene, Vector2i _size)
{
	scene = _scene;
	size = _size;
	tileSize.set(96, 96);

	for(int y = 0; y < size[1]; y++)
	{
		for(int x = 0; x < size[0]; x++)
		{
			Tile tile;
			if(x == 0 || y == 0 || x == size[0] - 1 || y == size[1] - 1)
			{
				tile.type = 1;
			}
			else
			{
				if(rand() % 5 == 0)
				{
					tile.type = 1;
				}
				else
				{
					tile.type = 0;
				}
			}
			tiles.push_back(tile);
		}
	}
	for(int y = 0; y < size[1]; y += patchSize)
	{
		for(int x = 0; x < size[0]; x += patchSize)
		{
			OwnPtr<kit::flat::Object> patch;
			patch.create(scene);
			patch->setTexture("art/tiles.png");
			patch->setPosition(Vector2i(x * tileSize[0], y * tileSize[1]));
			patchSprites.push_back(patch);
			updatePatch(Vector2i(x, y));
		}
	}

	paused = true;
}

Ptr<kit::scene::Scene> Level::getScene() const
{
	return scene;
}

void Level::addObject(OwnPtr<Object> const & object)
{
	objects.insert(object);
}

void Level::removeObject(Object const * object)
{
	// Clear linked objects
	if(object->getType() == Object::CHARACTER)
	{
		for(auto other : objects)
		{
			if(other->getType() == Object::CHAIN)
			{
				if(other.as<Chain>()->holdsCharacter((Character const *)object))
				{
					removeObject(other.raw());
				}
			}
		}
	}
	auto it = objects.find(object);
	if(it != objects.end())
	{
		objects.erase(*it);
	}
}

Tile & Level::getTile(Vector2i position)
{
	return tiles[position[1] * size[0] + position[0]];
}

Tile const & Level::getTile(Vector2i position) const
{
	return tiles[position[1] * size[0] + position[0]];
}

void Level::setPaused(bool _paused)
{
	paused = _paused;
}

void Level::update(float dt)
{
	if(paused)
	{
		return;
	}
	if(kit::math::random(0.f, 1.f) < dt * .05f)
	{
		kit::audio::play("sounds/announcer" + std::to_string(kit::math::random(0, 4)) + ".ogg");
	}
	if((signed)objects.size() < size[0] * size[1] / 20)
	{
		unsigned int itemType = kit::math::random(0, Object::NUM_TYPES);
		OwnPtr<Object> item;
		switch(itemType)
		{
			case Object::SWORD:
				item.create<Sword>(game->level);
				break;
			case Object::ROCKET_LAUNCHER:
			{
				int type = kit::math::random(0, RocketLauncher::NUM_ROCKET_LAUNCHER_TYPES);
				item.create<RocketLauncher>(game->level, type);
				break;
			}
			case Object::CHAIN_WAND:
				item.create<ChainWand>(game->level);
				break;
			case Object::CRAZY:
				item.create<Crazy>(game->level);
				break;
			case Object::SHRINKER:
				item.create<Shrinker>(game->level);
				break;
			case Object::SHELL:
				item.create<Shell>(-1, game->level, Shell::GREEN);
				break;
			case Object::NUKE:
				if(kit::math::random(0, 1) < .05f)
				{
					item.create<Nuke>(game->level);
				}
				break;
		}
		if(item.isValid())
		{
			Vector2i position = {kit::math::random(0, size[0]), kit::math::random(1, size[1])};
			item->setPosition(Vector2f(position[0] + .5f, position[1] + .5f).scale(tileSize));
			objects.insert(item);
		}
	}
	for(auto object : objects)
	{
		object->update(dt);
	}
	objects.processErases();
	for(auto object : objects)
	{
		object->doPhysics(dt);
	}
	objects.processErases();
	for(auto object0 : objects)
	{
		for(auto object1 : objects)
		{
			if(object0 == object1)
			{
				continue;
			}
			Vector2f r = object0->getPosition() - object1->getPosition();
			float radiusSum = object0->getRadius() * object0->getScale() + object1->getRadius() * object1->getScale();
			float d = (radiusSum * radiusSum) - r.normSq();
			if(d > 0 && !r.isZero())
			{
				if(object0->isSolid() && object1->isSolid())
				{
					d = radiusSum - r.norm();
					r.normalize();
					object0->setPosition(object0->getPosition() + r * d * 0.0f);
					object1->setPosition(object1->getPosition() - r * d * 0.0f);
					object0->applyImpulse(r * 10.f);
					object1->applyImpulse(-r * 10.f);
				}
				object0->onTouch(object1);
				object1->onTouch(object0);
			}
		}
	}
	objects.processErases();
	for(auto object : objects)
	{
		Vector2i position = Vector2i(object->getPosition()).scaleInv(tileSize);
		Vector2i start = Vector2i(object->getPosition()).scaleInv(tileSize);
		Vector2i tilePosition;
		for(tilePosition[1] = position[1] - 1; tilePosition[1] <= position[1] + 1; tilePosition[1]++)
		{
			for(tilePosition[0] = position[0] - 1; tilePosition[0] <= position[0] + 1; tilePosition[0]++)
			{
				if(tilePosition[0] < 0 || tilePosition[1] < 0 || tilePosition[0] >= size[0] || tilePosition[1] >= size[1])
				{
					continue; // out of bounds
				}
				Tile const & tile = getTile(tilePosition);
				Rectf bounds = Rectf::minSize((float)(tilePosition[0] * tileSize[0]), (float)(tilePosition[1] * tileSize[1]), (float)tileSize[0], (float)tileSize[1]);
				// Get closest point to circle within tile
				Vector2f closest = bounds.closest(object->getPosition());
				if((closest - object->getPosition()).normSq() < object->getRadius() * object->getScale() * object->getRadius() * object->getScale())
				{
					object->onOverTile(tilePosition, closest);
					if(tile.type == Tile::Floor)
					{
						continue;
					}
					Vector2f r = object->getPosition() - closest;
					if(!r.isZero())
					{
						Vector2f rUnit = r.unit();
						object->setPosition(object->getPosition() + rUnit * (object->getRadius() * object->getScale() - r.norm()));
						object->setVelocity(object->getVelocity() - object->getBounciness() * rUnit * object->getVelocity().dot(rUnit));
					}
				}
			}
		}
	}
	objects.processErases();
}

void Level::preRenderUpdate()
{
	for(auto object : objects)
	{
		object->preRenderUpdate();
	}
}

std::vector<std::pair<Ptr<Object>, float>> Level::getObjectsWithinCircle(Vector2f position, float radius, Object const * exclude) const
{
	float radiusSq = radius * radius;
	std::vector<std::pair<Ptr<Object>, float>> objectsWithin;
	for(auto object : objects)
	{
		if(exclude == object.raw())
		{
			continue;
		}
		float distanceSq = (object->getPosition() - position).normSq();
		if(distanceSq < radiusSq)
		{
			objectsWithin.push_back(std::pair<Ptr<Object>, float>(object, std::sqrtf(distanceSq)));
		}
	}
	return objectsWithin;
}

std::pair<Ptr<Object>, float> Level::getNearestObject(Vector2f position, Object const * exclude) const
{
	Ptr<Object> nearestObject;
	float nearestDistanceSq = 0;
	for(auto object : objects)
	{
		if(exclude == object.raw())
		{
			continue;
		}
		float distanceSq = (object->getPosition() - position).normSq();
		if(!nearestObject.isValid() || distanceSq < nearestDistanceSq)
		{
			nearestObject = object;
			nearestDistanceSq = distanceSq;
		}
	}
	return std::pair<Ptr<Object>, float>(nearestObject, std::sqrtf(nearestDistanceSq));
}

void Level::updatePatch(Vector2i tile)
{
	int patchIndex = (tile[1] / patchSize) * kit::math::ceil(size[0], patchSize) + (tile[0] / patchSize);
	Vector2i start = Vector2i(tile[0] / patchSize * patchSize, tile[1] / patchSize * patchSize);
	Vector2i end = Vector2i(kit::math::min(start[0] + patchSize, size[0]), kit::math::min(start[1] + patchSize, size[1]));

	Vector2f textureSize = patchSprites[patchIndex]->getTexture()->getSize();
	std::vector<kit::flat::Object::Vertex> vertices;
	std::vector<unsigned int> indices;
	kit::flat::Object::Vertex v;
	for(int y = start[1]; y < end[1]; y++)
	{
		for(int x = start[0]; x < end[0]; x++)
		{
			indices.push_back(0 + vertices.size());
			indices.push_back(1 + vertices.size());
			indices.push_back(2 + vertices.size());
			indices.push_back(2 + vertices.size());
			indices.push_back(3 + vertices.size());
			indices.push_back(0 + vertices.size());
			Tile const & tile = tiles[y * size[0] + x];
			v.position.set((float)((x - start[0]) * tileSize[0]), (float)((y - start[1]) * tileSize[1]));
			v.textureCoord.set((float)(tileSize[0] * tile.type) / textureSize[0], (float)(tileSize[1]) / textureSize[1]);
			vertices.push_back(v);
			v.position[0] += tileSize[0];
			v.textureCoord[0] += tileSize[0] / textureSize[0];
			vertices.push_back(v);
			v.position[1] += tileSize[1];
			v.textureCoord[1] -= tileSize[1] / textureSize[1];
			vertices.push_back(v);
			v.position[0] -= tileSize[0];
			v.textureCoord[0] -= tileSize[0] / textureSize[0];
			vertices.push_back(v);
		}
	}
	patchSprites[patchIndex]->setVertices(vertices);
	patchSprites[patchIndex]->setIndices(indices);
}

