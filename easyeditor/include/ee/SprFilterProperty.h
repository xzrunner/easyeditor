#ifndef _EASYEDITOR_SPR_FILTER_PROPERTY_H_
#define _EASYEDITOR_SPR_FILTER_PROPERTY_H_

#include <wx/propgrid/propgrid.h>

namespace s2 { class RenderFilter; }

namespace ee
{

class Sprite;

class SprFilterProperty
{
public:
	SprFilterProperty(wxWindow* parent);

	void InitPS(const Sprite* spr, wxPropertyGrid* pg);
	bool FromPS(const std::string& name, const wxAny& value, Sprite* spr);
	void ToPS(const Sprite* spr, wxPropertyGrid* pg);

private:
	void CreateSubPS(wxPropertyGrid* pg, wxPGProperty* parent, const s2::RenderFilter* filter);

private:
	wxWindow* m_parent;

}; // SprFilterProperty

}

#endif // _EASYEDITOR_SPR_FILTER_PROPERTY_H_