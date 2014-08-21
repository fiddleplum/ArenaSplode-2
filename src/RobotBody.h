#pragma once

#include "Object.h"

class Player;

class RobotBody : public Object
{
public:
	RobotBody(Ptr<Level> level, std::string const & characterFilename, int i);

};

