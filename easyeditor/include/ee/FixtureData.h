#ifndef _EASYEDITOR_FIXTURE_DATA_H_
#define _EASYEDITOR_FIXTURE_DATA_H_

#include "Vector.h"

#include <string>

namespace ee
{

struct FixtureDataInfo
{
	std::string filepath;
	Vector pos;
	float angle;
	float scale;

}; // FixtureDataInfo

}

#endif // _EASYEDITOR_FIXTURE_DATA_H_