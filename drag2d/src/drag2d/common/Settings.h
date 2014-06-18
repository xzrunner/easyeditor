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
		static bool bVisibleNodeName;

		enum DrawType
		{
			e_DrawBoth = 0,
			e_DrawPhysics,
			e_DrawImage
		};
		static DrawType drawType;

		static const int DrawFontBg   = 0x1;
		static const int DrawFontText = 0x2;
		static int DrawFontType;

		static std::set<wxString> RESOURCE_PATH;

	}; // Settings
}

