#pragma once

#include <kit/flat/object.h>

class Player;

class Object
{
public:
	enum Type { CHARACTER, SWORD, ROCKET };

	Object(Type type, Ptr<kit::scene::Scene> scene, std::string const & textureFilename, Recti textureCoords);

	Type getType() const { return type; }

	Vector2f getPosition() const { return object->getPosition(); }

	float getOrientation() const { return object->getOrientation(); }

	int getZ() const { return object->getZ(); }

	bool isSolid() const { return solid; }

	void setSolid(bool _solid) { solid = _solid; }

	float getBounciness() const { return bounciness; }

	void setBounciness(float bounciness);

	float getFriction() const { return friction; }
	void setFriction(float _friction) { friction = _friction; }

	float getRadius() const { return radius; }

	Vector2f getVelocity() const { return velocity; }

	void setVelocity(Vector2f velocity);

	void doPhysics(float dt);

	void setPosition(Vector2f position);

	void setOrientation(float orientation);

	void setZ(int z);

	void setHeld(Ptr<Player> player) { holdingPlayer = player; }

	bool isHeld() const { return holdingPlayer.isValid(); }

	virtual void update(float dt) = 0;

private:
	Ptr<kit::scene::Scene> scene;
	OwnPtr<kit::flat::Object> object;
	Type type;
	bool solid;
	float bounciness;
	float radius;
	Vector2f velocity;
	float friction;
	Ptr<Player> holdingPlayer;
};

