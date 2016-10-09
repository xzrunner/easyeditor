#include "SprTagProperty.h"
#include "FileHelper.h"
#include "SprTagEditDlg.h"
#include "Sprite.h"

#include <wx/filedlg.h>

namespace ee
{

WX_PG_IMPLEMENT_PROPERTY_CLASS(SprTagProperty, wxLongStringProperty, wxSize, const wxSize&, TextCtrlAndButton)

SprTagProperty::SprTagProperty(const wxString& label, const wxString& name, const wxString& value)
	: wxLongStringProperty(label, name, value)
	, m_parent(NULL)
	, m_spr(NULL)
{
}

bool SprTagProperty::OnButtonClick(wxPropertyGrid* propgrid, wxString& value)
{
	SprTagEditDlg dlg(m_parent, m_spr);
	if (dlg.ShowModal() == wxID_OK) {
		m_spr->SetTag(dlg.ToString());
		return true;
	} else {
		return false;
	}
}

}