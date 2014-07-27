#include "Level.h"
#include "Sword.h"
#include "RocketLauncher.h"
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
			if(rand() % 5 == 0)
			{
				tile.type = 1;
			}
			else
			{
				tile.type = 0;
			}
			tiles.push_back(tile);
		}
	}
	for(int y = 0; y < size[1]; y += patchSize)
	{
		for(int x = 0; x < size[0]; x += patchSize)
		{
			OwnPtr<kit::flat::Object> patch;
			patch.setNew(scene);
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

void Level::addObject(OwnPtr<Object> object)
{
	objects.insert(object);
}

void Level::removeObject(Ptr<Object> object)
{
	objects.erase(object);
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
	if((signed)objects.size() < size[0] * size[1] / 20)
	{
		unsigned int itemType = kit::math::random(0, Object::NUM_TYPES);
		OwnPtr<Object> item;
		switch(itemType)
		{
		case Object::SWORD:
			item.setRaw(new Sword(game->level));
			break;
		case Object::ROCKET_LAUNCHER:
			item.setRaw(new RocketLauncher(game->level));
			break;
		}
		if(item.isValid())
		{
			Vector2i position = { kit::math::random(0, size[0]), kit::math::random(1, size[1]) };
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
			if(object0 == object1 || (!object0->isSolid() && object0->getHeldCharacter().isNull()) || (!object1->isSolid() && object1->getHeldCharacter().isNull()))
			{
				continue;
			}
			Vector2f r = object0->getPosition() - object1->getPosition();
			float radiusSum = object0->getRadius() + object1->getRadius();
			float d = (radiusSum * radiusSum) - r.normSq();
			if(d > 0 && !r.isZero())
			{
				d = radiusSum - r.norm();
				r.normalize();
				object0->setPosition(object0->getPosition() + r * d * 0.0f);
				object1->setPosition(object1->getPosition() - r * d * 0.0f);
				object0->onTouch(object1);
				object1->onTouch(object0);
			}
		}
		objects.processErases();
	}
	for(auto object : objects)
	{
		Vector2i position = Vector2i(object->getPosition()).scaleInv(tileSize);
		Vector2i start = Vector2i(object->getPosition()).scaleInv(tileSize);
		for(int y = position[1] - 1; y <= position[1] + 1; y++)
		{
			for(int x = position[0] - 1; x <= position[0] + 1; x++)
			{
				if(x < 0 || y < 0 || x >= size[0] || y >= size[1])
				{
					continue;
				}
				Tile const & tile = tiles[y * size[0] + x];
				if(tile.type == Tile::Floor)
				{
					continue;
				}
				Rectf bounds = Rectf::minSize((float)(x * tileSize[0]), (float)(y * tileSize[1]), (float)tileSize[0], (float)tileSize[1]);
				// Get closest point to circle within tile
				Vector2f closest = bounds.closest(object->getPosition());

				if((closest - object->getPosition()).normSq() < object->getRadius() * object->getRadius())
				{
					Vector2f r = object->getPosition() - closest;
					if(!r.isZero())
					{
						Vector2f rUnit = r.unit();
						object->setPosition(object->getPosition() + rUnit * (object->getRadius() - r.norm()));
						if(r.normSq() > .1f)
						{
							object->setVelocity(object->getVelocity() - 1.0f * rUnit * object->getVelocity().dot(rUnit));
						}
						else
						{
							object->setVelocity(object->getVelocity() - rUnit * object->getVelocity().dot(rUnit));
						}
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

std::pair<Ptr<Object>, float> Level::getNearestObject(Ptr<Object> reference)
{
	Ptr<Object> nearestObject;
	float nearestDistanceSq;
	for(auto object : objects)
	{
		if(object == reference)
		{
			continue;
		}
		float distanceSq = (object->getPosition() - reference->getPosition()).normSq();
		if(nearestObject.isNull() || distanceSq < nearestDistanceSq)
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

