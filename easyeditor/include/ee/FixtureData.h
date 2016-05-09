#ifndef _EASYEDITOR_FIXTURE_DATA_H_
#define _EASYEDITOR_FIXTURE_DATA_H_

#include <string>

#include <SM_Vector.h>

namespace ee
{

struct FixtureDataInfo
{
	std::string filepath;
	sm::vec2 pos;
	float angle;
	float scale;

}; // FixtureDataInfo

}

#endif // _EASYEDITOR_FIXTURE_DATA_H_