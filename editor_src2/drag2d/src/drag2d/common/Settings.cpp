#include "Settings.h"

namespace d2d
{

bool Settings::bZoomOnlyUseMouseWheel = false;

int Settings::ctlPosSize = 0;
bool Settings::bDisplayTrisEdge = false;
bool Settings::bDisplayPolyBound = true;

Settings::DrawType Settings::drawType = e_DrawBoth;

std::set<wxString> Settings::RESOURCE_PATH;

} // d2d