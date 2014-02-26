#pragma once

#include <wx/wx.h>

#include <set>

namespace d2d
{
	class Settings
	{
	public:
		static int ctlPosSize;
		static bool bDisplayTrisEdge, bDisplayPolyBound;

		enum DrawType
		{
			e_DrawBoth = 0,
			e_DrawPhysics,
			e_DrawImage
		};
		static DrawType drawType;

		static std::set<wxString> RESOURCE_PATH;

	}; // Settings
}

