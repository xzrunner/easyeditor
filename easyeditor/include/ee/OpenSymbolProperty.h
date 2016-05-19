#ifndef _EASYEDITOR_OPEN_SYMBOL_PROPERTY_H_
#define _EASYEDITOR_OPEN_SYMBOL_PROPERTY_H_

#include <wx/propgrid/propgrid.h>

namespace ee
{

class OpenSymbolMonitor;

class OpenSymbolProperty : public wxLongStringProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(OpenSymbolProperty)

public:
	OpenSymbolProperty(const wxString& label = wxPG_LABEL, 
		const wxString& name = wxPG_LABEL, const wxString& value = wxPG_LABEL);
	virtual ~OpenSymbolProperty();

	virtual wxVariant ChildChanged( wxVariant& thisValue,
		int childIndex,
		wxVariant& childValue ) const;
	virtual void RefreshChildren();

	virtual bool OnButtonClick( wxPropertyGrid* propgrid, wxString& value );

	void SetParent(wxWindow* parent) { m_parent = parent; }
 	void SetListener(OpenSymbolMonitor* lsn);

private:
	wxWindow* m_parent;

	OpenSymbolMonitor* m_lsn;

}; // OpenSymbolProperty

}

#endif // _EASYEDITOR_OPEN_SYMBOL_PROPERTY_H_