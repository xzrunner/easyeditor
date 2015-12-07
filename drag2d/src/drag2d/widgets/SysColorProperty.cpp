#include "SysColorProperty.h"
#include "IColorMonitor.h"

#include "common/Color.h"

#include <wx/colordlg.h>

namespace d2d
{

WX_PG_IMPLEMENT_PROPERTY_CLASS(SysColorProperty,wxLongStringProperty,wxSize,const wxSize&,TextCtrlAndButton)

SysColorProperty::SysColorProperty(const wxString& label, const wxString& name) 
	: wxLongStringProperty(label, name)
	, m_lsn(NULL)
	, m_parent(NULL)
	, m_color_data(NULL)
{}

SysColorProperty::~SysColorProperty() 
{}

bool SysColorProperty::OnButtonClick( wxPropertyGrid* propGrid, wxString& value )
{
	if (!m_lsn) {
		return false;
	}

	wxString str = m_color_data->ToString();

	wxColourDialog dlg(m_parent, m_color_data);
	dlg.SetTitle(wxT("Choose Color"));
	if (dlg.ShowModal() == wxID_OK)
	{
		*m_color_data = dlg.GetColourData();
		const wxColor& col = dlg.GetColourData().GetColour();
		m_lsn->OnColorChanged(Colorf(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f));
	}

	return false;
}

void SysColorProperty::SetListener(IColorMonitor* lsn) 
{ 
	if (m_lsn) {
		delete m_lsn;
	}
	m_lsn = lsn; 
}

}