#ifndef _EPHYSICS_FIXTURE_DATA_H_
#define _EPHYSICS_FIXTURE_DATA_H_

#include <wx/string.h>


namespace ephysics
{

struct FixtureDataInfo
{
	std::string filepath;
	ee::Vector pos;
	float angle;
	float scale;

}; // FixtureDataInfo

}

#endif // _EPHYSICS_FIXTURE_DATA_H_