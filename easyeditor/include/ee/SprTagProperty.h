#ifndef _EASYEDITOR_SPR_TAG_PROPERTY_H_
#define _EASYEDITOR_SPR_TAG_PROPERTY_H_

#include <wx/propgrid/propgrid.h>

namespace ee
{

class Sprite;

class SprTagProperty : public wxLongStringProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(SprTagProperty)

public:
	SprTagProperty(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL, 
		const wxString& value = wxEmptyString);

	virtual bool OnButtonClick(wxPropertyGrid* propgrid, wxString& value);

	void SetParent(wxWindow* parent) { m_parent = parent; }

	void SetSprite(Sprite* spr) { m_spr = spr; }

private:
	wxWindow* m_parent;

	Sprite* m_spr;

}; // SprTagProperty

}

#endif // _EASYEDITOR_SPR_TAG_PROPERTY_H_