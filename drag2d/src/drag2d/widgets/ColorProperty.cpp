#include "ColorProperty.h"
#include "IColorMonitor.h"
#include "HSLColorSettingDlg.h"

#include "view/EditPanel.h"

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

	wxSize dialogSize(450, 300);
	wxPoint dlgPos = propGrid->GetGoodEditorDialogPosition(this, dialogSize);

	HSLColorSettingDlg dlg(m_parent, m_lsn, m_lsn->GetColor(), dlgPos);
	dlg.ShowModal();

	m_lsn->OnColorChanged(dlg.GetColor());

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