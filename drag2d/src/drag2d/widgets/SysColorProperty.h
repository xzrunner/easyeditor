#ifndef _DRAG2D_SYS_COLOR_PROPERTY_H_
#define _DRAG2D_SYS_COLOR_PROPERTY_H_

#include <wx/propgrid/propgrid.h>
#include <wx/colordlg.h>

namespace d2d
{

class IColorMonitor;
struct Colorf;

class SysColorProperty : public wxLongStringProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(SysColorProperty)

public:
	SysColorProperty(const wxString& label = wxPG_LABEL, 
		const wxString& name = wxPG_LABEL);
	virtual ~SysColorProperty();

	virtual bool OnButtonClick( wxPropertyGrid* propgrid, wxString& value );

	void SetParent(wxWindow* parent) { m_parent = parent; }
	void SetListener(IColorMonitor* lsn);

	void SetColorData(wxColourData* cd) { m_color_data = cd; }

private:
	wxWindow* m_parent;

	IColorMonitor* m_lsn;

	wxColourData* m_color_data;

}; // SysColorProperty

}

#endif // _DRAG2D_SYS_COLOR_PROPERTY_H_