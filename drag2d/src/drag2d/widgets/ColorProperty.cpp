#include "ColorProperty.h"
#include "IColorMonitor.h"
#include "RGBColorSettingDlg.h"
#include "HSLColorSettingDlg.h"

#include "common/Config.h"
#include "common/SettingData.h"

namespace d2d
{

WX_PG_IMPLEMENT_PROPERTY_CLASS(ColorProperty,wxLongStringProperty,wxSize,const wxSize&,TextCtrlAndButton)

ColorProperty::ColorProperty(const wxString& label, const wxString& name) 
	: wxLongStringProperty(label, name)
	, m_lsn(NULL)
	, m_parent(NULL)
{}

ColorProperty::~ColorProperty() 
{}

void ColorProperty::RefreshChildren()
{
//     if ( !GetChildCount() ) return;
//     const wxSize& size = wxSizeRefFromVariant(m_value);
//     Item(0)->SetValue( (long)size.x );
//     Item(1)->SetValue( (long)size.y );
}

wxVariant ColorProperty::ChildChanged( wxVariant& thisValue,
                                        int childIndex,
                                        wxVariant& childValue ) const
{
// 	if (m_color) {
// //		return wxColour(m_color->r*255, m_color->g*255, m_color->b*255, m_color->a*255);
// 		return wxNullColour;
// 	} else {
		return wxNullColour;
//	}
}

bool ColorProperty::OnButtonClick( wxPropertyGrid* propGrid, wxString& value )
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
// 		m_stage->ResetViewport();
// 		return true;
// 	}

	return false;
}

void ColorProperty::SetListener(IColorMonitor* lsn) 
{ 
	if (m_lsn) {
		delete m_lsn;
	}
	m_lsn = lsn; 
}

}