#ifndef _EASYEDITOR_FIXTURE_DATA_H_
#define _EASYEDITOR_FIXTURE_DATA_H_

#include <cu/cu_stl.h>

#include <SM_Vector.h>

namespace ee
{

struct FixtureDataInfo
{
	CU_STR filepath;
	sm::vec2 pos;
	float angle;
	float scale;

}; // FixtureDataInfo

}

#endif // _EASYEDITOR_FIXTURE_DATA_H_