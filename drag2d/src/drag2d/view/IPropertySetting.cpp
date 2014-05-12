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

void IPropertySetting::splitString(const wxAny& value, double* x, double* y)
{
	wxString val = wxANY_AS(value, wxString);
	size_t gap = val.find_first_of(';');
	wxString sx = val.substr(0, gap),
		sy = val.substr(gap + 1);
	sx.ToDouble(x);
	sy.ToDouble(y);
}

} // d2d