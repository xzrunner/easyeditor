#ifndef _EPHYSICS_FIXTURE_DATA_H_
#define _EPHYSICS_FIXTURE_DATA_H_

#include <wx/string.h>
#include <drag2d.h>

namespace ephysics
{

struct FixtureDataInfo
{
	wxString filepath;
	d2d::Vector pos;
	float angle;
	float scale;

}; // FixtureDataInfo

}

#endif // _EPHYSICS_FIXTURE_DATA_H_