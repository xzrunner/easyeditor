#include "MyColorProperty.h"
#include "ColorMonitor.h"
#include "RGBColorSettingDlg.h"
#include "HSLColorSettingDlg.h"
#include "Config.h"
#include "SettingData.h"

namespace ee
{

WX_PG_IMPLEMENT_PROPERTY_CLASS(MyColorProperty,wxLongStringProperty,wxSize,const wxSize&,TextCtrlAndButton)

MyColorProperty::MyColorProperty(const wxString& label, const wxString& name) 
	: wxLongStringProperty(label, name)
	, m_lsn(NULL)
	, m_parent(NULL)
{}

MyColorProperty::~MyColorProperty() 
{}

void MyColorProperty::RefreshChildren()
{
//     if ( !GetChildCount() ) return;
//     const wxSize& size = wxSizeRefFromVariant(m_value);
//     Item(0)->SetValue( (long)size.x );
//     Item(1)->SetValue( (long)size.y );
}

wxVariant MyColorProperty::ChildChanged(wxVariant& thisValue,
                                        int childIndex,
                                        wxVariant& childValue ) const
{
// 	if (m_color) {
// //		return wxColour(m_color->r, m_color->g, m_color->b, m_color->a);
// 		return wxNullColour;
// 	} else {
		return wxNullColour;
//	}
}

bool MyColorProperty::OnButtonClick( wxPropertyGrid* propGrid, wxString& value )
{
	if (!m_lsn) {
		return false;
	}

	wxSize dlg_sz(450, 300);
	wxPoint dlg_pos = propGrid->GetGoodEditorDialogPosition(this, dlg_sz);
	if (Config::Instance()->GetSettings().color_setting_dlg_type == CSDT_RGB) {
		RGBColorSettingDlg dlg(m_parent, m_lsn, m_lsn->GetColor(), dlg_pos);
		dlg.ShowModal();
		m_lsn->OnColorChanged(dlg.GetColor());
	} else {
		HSLColorSettingDlg dlg(m_parent, m_lsn, m_lsn->GetColor(), dlg_pos);
		dlg.ShowModal();
		m_lsn->OnColorChanged(dlg.GetColor());
	}

//	// todo
// 	if ( dlg.ShowModal() == wxID_OK )
// 	{
// 		*m_color = dlg.GetColor();
// 		return true;
// 	}

	return false;
}

void MyColorProperty::SetListener(ColorMonitor* lsn) 
{ 
	if (m_lsn) {
		delete m_lsn;
	}
	m_lsn = lsn; 
}

}