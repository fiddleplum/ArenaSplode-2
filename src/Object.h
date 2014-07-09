#pragma once

#include <kit/flat/object.h>

using namespace kit;

class Object
{
public:
	Object(Ptr<scene::Scene> scene, std::string const & textureFilename, Recti textureCoords);

	Vector2f getPosition() const { return _object->getPosition() - _objectOffset; }

	float getOrientation() const { return _object->getOrientation(); }

	bool isSolid() const { return _solid; }

	void setSolid(bool solid) { _solid = solid; }

	float getRadius() const { return _radius; }

	Vector2f getVelocity() const { return _velocity; }

	void setVelocity(Vector2f velocity);

	void doPhysics(float dt);

	void setPosition(Vector2f position);

private:
	Ptr<scene::Scene> _scene;
	OwnPtr<flat::Object> _object;
	Vector2f _objectOffset;
	bool _solid;
	float _radius;
	Vector2f _velocity;
	float _friction;
};

