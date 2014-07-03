#include "Settings.h"

namespace d2d
{

int Settings::ctlPosSize = 0;
bool Settings::bDisplayTrisEdge = false;
bool Settings::bDisplayPolyBound = true;
bool Settings::bSpriteCapture = true;
bool Settings::bImageEdgeClip = true;
bool Settings::bVisibleImgEdge = false;
bool Settings::bVisibleNodeName = false;

Settings::DrawType Settings::drawType = e_DrawBoth;

int Settings::DrawFontType = DrawFontBg | DrawFontText;

std::set<wxString> Settings::RESOURCE_PATH;

} // d2d