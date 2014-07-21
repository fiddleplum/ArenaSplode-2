#pragma once

#include "Object.h"

class Character : public Object
{
public:
	Character(Ptr<kit::scene::Scene> scene, std::string const & characterFilename);

	Ptr<Object> getObjectHeld() const { return objectHeld; }
	void setObjectHeld(Ptr<Object> object);

private:
	Ptr<Object> objectHeld;
};

