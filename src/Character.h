#pragma once

#include "Object.h"

class Character : public Object
{
public:
	Character(Ptr<Level> level, std::string const & characterFilename);

	Ptr<Object> getObjectHeld() const { return objectHeld; }
	void setObjectHeld(Ptr<Object> object);

	void useHeld();

	void update(float dt) override;
	void preRenderUpdate() override;

private:
	Ptr<Object> objectHeld;

	bool swinging;
	float heldOrientationOffset;
	float heldRadiusOffset;
};

