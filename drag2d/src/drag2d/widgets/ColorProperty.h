#ifndef _DRAG2D_COLOR_PROPERTY_H_
#define _DRAG2D_COLOR_PROPERTY_H_

#include <wx/propgrid/propgrid.h>

namespace d2d
{

class EditPanel;
struct Colorf;

class ColorProperty : public wxLongStringProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(wxTriangleProperty)

public:
	ColorProperty(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL);
	virtual ~ColorProperty();

	virtual wxVariant ChildChanged( wxVariant& thisValue,
		int childIndex,
		wxVariant& childValue ) const;
	virtual void RefreshChildren();

	virtual bool OnButtonClick( wxPropertyGrid* propgrid, wxString& value );

	void SetStage(EditPanel* stage) { m_stage = stage; }
	void SetColor(Colorf* color) { m_color = color; }

private:
	EditPanel* m_stage;

	Colorf* m_color;

}; // ColorProperty

}

#endif // _DRAG2D_COLOR_PROPERTY_H_