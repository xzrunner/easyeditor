#pragma once

#include "common/Vector.h"

#include <wx/wx.h>

namespace d2d
{
	struct FixtureDataInfo
	{
		wxString filepath;
		Vector pos;
		float angle;
		float scale;
	}; // FixtureDataInfo
}

