#include "ColorProperty.h"
#include "HSLColorSettingDlg.h"

#include "view/EditPanel.h"

namespace d2d
{

WX_PG_IMPLEMENT_PROPERTY_CLASS(ColorProperty,wxLongStringProperty,wxSize,const wxSize&,TextCtrlAndButton)

ColorProperty::ColorProperty(const wxString& label, const wxString& name) 
	: wxLongStringProperty(label, name)
	, m_stage(NULL)
	, m_color(NULL)
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
	if (m_color) {
		return wxColour(m_color->r*255, m_color->g*255, m_color->b*255, m_color->a*255);
	} else {
		return wxNullColour;
	}
}

bool ColorProperty::OnButtonClick( wxPropertyGrid* propGrid, wxString& value )
{
	wxSize dialogSize(450, 300);
	wxPoint dlgPos = propGrid->GetGoodEditorDialogPosition(this, dialogSize);

	HSLColorSettingDlg dlg(m_stage, *m_color, dlgPos);
	dlg.ShowModal();
	*m_color = dlg.GetColor();
	m_stage->ResetViewport();

//	// todo
// 	if ( dlg.ShowModal() == wxID_OK )
// 	{
// 		*m_color = dlg.GetColor();
// 		m_stage->ResetViewport();
// 		return true;
// 	}

	return false;
}

}