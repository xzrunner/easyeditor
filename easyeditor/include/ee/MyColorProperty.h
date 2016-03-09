#ifndef _EASYEDITOR_MY_COLOR_PROPERTY_H_
#define _EASYEDITOR_MY_COLOR_PROPERTY_H_

#include <wx/propgrid/propgrid.h>

namespace ee
{

class ColorMonitor;
class Colorf;

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
	void SetListener(ColorMonitor* lsn);

private:
	wxWindow* m_parent;

	ColorMonitor* m_lsn;

}; // MyColorProperty

}

#endif // _EASYEDITOR_MY_COLOR_PROPERTY_H_