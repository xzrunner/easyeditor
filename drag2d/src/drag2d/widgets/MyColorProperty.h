#ifndef _DRAG2D_MY_COLOR_PROPERTY_H_
#define _DRAG2D_MY_COLOR_PROPERTY_H_

#include <wx/propgrid/propgrid.h>

namespace d2d
{

class IColorMonitor;
struct Colorf;

class MyColorProperty : public wxLongStringProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(MyColorProperty)

public:
	MyColorProperty(const wxString& label = wxPG_LABEL, 
		const wxString& name = wxPG_LABEL);
	virtual ~MyColorProperty();

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

}; // MyColorProperty

}

#endif // _DRAG2D_MY_COLOR_PROPERTY_H_