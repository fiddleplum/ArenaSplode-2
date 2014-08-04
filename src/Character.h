#pragma once

#include "Object.h"

class Character : public Object
{
public:
	Character(Ptr<Level> level, std::string const & characterFilename);

	float getMaxSpeed() const { return maxSpeed; }
	void setMaxSpeed(float _maxSpeed) { maxSpeed = _maxSpeed; }

	Ptr<Object> getObjectHeld() const { return objectHeld; }
	void setObjectHeld(Ptr<Object> object);

	bool isSwinging() { return swinging; }

	void useHeld();

	void harm(float amount);

	void die();

	void update(float dt) override;
	void preRenderUpdate() override;

private:
	Ptr<Object> objectHeld;

	float maxSpeed;

	bool swinging;
	float heldOrientationOffset;
	float heldRadiusOffset;

	float health;
};

