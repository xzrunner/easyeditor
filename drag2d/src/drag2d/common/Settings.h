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

		static bool bSpriteCapture;
		static bool bImageEdgeClip;
		static bool bVisibleImgEdge;

		enum DrawType
		{
			e_DrawBoth = 0,
			e_DrawPhysics,
			e_DrawImage
		};
		static DrawType drawType;

		enum DrawFontType
		{
			e_DrawFontBg = 0,
			e_DrawFontText,
			e_DrawFontAll,
			e_DrawFontNull
		};
		static DrawFontType drawFontType;

		static std::set<wxString> RESOURCE_PATH;

	}; // Settings
}

