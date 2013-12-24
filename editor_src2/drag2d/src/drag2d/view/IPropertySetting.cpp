#include "IPropertySetting.h"
#include "PropertySettingPanel.h"

namespace d2d
{

const wxString IPropertySetting::TYPE_NAME = wxT("Type");

IPropertySetting::IPropertySetting(d2d::EditPanel* editPanel,
								   const wxString& type)
	: m_editPanel(editPanel)
	, m_type(type)
{
}

wxString IPropertySetting::getPGType(wxPropertyGrid* pg) const
{
	if (!pg) return wxEmptyString;
	wxPGProperty* property = pg->GetProperty(TYPE_NAME);
	if (!property) return wxEmptyString;
	wxAny value = property->GetValue();
	return wxANY_AS(value, wxString);
}

} // d2d