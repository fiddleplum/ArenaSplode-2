#include "RobotBody.h"
#include "Level.h"

RobotBody::RobotBody(Ptr<Level> level, std::string const & robotBodyFilename, int i)
	: Object(ROBOT_BODY, level, "art/characters/" + robotBodyFilename, Recti::minSize((i / 2) * 32, (i % 2) * 32, 32, 32))
{
	setRadius(16.f);
	setSolid(true);
	setZ(1);
}

