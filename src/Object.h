#pragma once

#include <kit/flat/object.h>

class Level;
class Character;

class Object
{
public:
	enum Type { CHARACTER, SWORD, ROCKET_LAUNCHER, ROCKET, NUM_TYPES };

	Object(Type type, Ptr<Level> level, std::string const & textureFilename, Recti textureCoords);

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

	void applyImpulse(Vector2f impulse);

	void doPhysics(float dt);

	void setPosition(Vector2f position);

	void setOrientation(float orientation);

	void setZ(int z);

	Ptr<Character> getHeldCharacter() const { return holdingCharacter; }

	void setHeld(Ptr<Character> character) { holdingCharacter = character; }

	virtual void update(float dt);

	virtual void preRenderUpdate();

	virtual void onTouch(Ptr<Object> object);

protected:
	Ptr<Level> level;

private:
	OwnPtr<kit::flat::Object> object;
	Type type;
	bool solid;
	float bounciness;
	float radius;
	Vector2f velocity;
	float friction;
	Ptr<Character> holdingCharacter;
};

