#ifndef _DRAG2D_COLOR_PROPERTY_H_
#define _DRAG2D_COLOR_PROPERTY_H_

#include <wx/propgrid/propgrid.h>

namespace d2d
{

class IColorMonitor;
struct Colorf;

class ColorProperty : public wxLongStringProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(wxTriangleProperty)

public:
	ColorProperty(const wxString& label = wxPG_LABEL, 
		const wxString& name = wxPG_LABEL);
	virtual ~ColorProperty();

	virtual wxVariant ChildChanged( wxVariant& thisValue,
		int childIndex,
		wxVariant& childValue ) const;
	virtual void RefreshChildren();

	virtual bool OnButtonClick( wxPropertyGrid* propgrid, wxString& value );

	void SetParent(wxWindow* parent) { m_parent = parent; }
	void SetListener(IColorMonitor* lsn);

private:
	wxWindow* m_parent;

	IColorMonitor* m_lsn;

}; // ColorProperty

}

#endif // _DRAG2D_COLOR_PROPERTY_H_