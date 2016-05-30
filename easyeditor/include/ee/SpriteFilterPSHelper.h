#ifndef _EASYEDITOR_SPRITE_FILTER_PS_HELPER_H_
#define _EASYEDITOR_SPRITE_FILTER_PS_HELPER_H_

#include <wx/propgrid/propgrid.h>

namespace s2 { class RenderFilter; }

namespace ee
{

class Sprite;

class SpriteFilterPSHelper
{
public:
	static void InitPS(const Sprite* spr, wxPropertyGrid* pg);
	static bool FromPS(const std::string& name, const wxAny& value, Sprite* spr);
	static void ToPS(const Sprite* spr, wxPropertyGrid* pg);

private:
	static void CreateSubPS(wxPropertyGrid* pg, wxPGProperty* parent, const s2::RenderFilter* filter);

}; // SpriteFilterPSHelper

}

#endif // _EASYEDITOR_SPRITE_FILTER_PS_HELPER_H_