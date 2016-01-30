#include "IPropertySetting.h"
#include "PropertySettingPanel.h"

namespace d2d
{

const wxString IPropertySetting::TYPE_NAME = wxT("Type");

IPropertySetting::IPropertySetting(const std::string& type)
	: m_type(type)
{
}

void IPropertySetting::UpdateFromPanel(PropertySettingPanel* panel)
{
	wxPropertyGrid* pg = panel->GetPropertyGrid();
	if (panel->GetType() == m_type) {
		UpdateProperties(pg);
	} else {
		InitProperties(pg);
	}
}

void IPropertySetting::EnablePropertyGrid(PropertySettingPanel* panel, bool enable)
{
	wxPropertyGrid* pg = panel->GetPropertyGrid();

	if (panel->GetType() != m_type) {
		InitProperties(pg);
	}

	for (wxPropertyGridIterator itr = pg->GetIterator(wxPG_ITERATE_ALL); !itr.AtEnd(); itr++) {
		wxPGProperty* p = *itr;
		p->Enable(enable);
	}
}

wxString IPropertySetting::GetPGType(wxPropertyGrid* pg) const
{
	if (!pg) return wxEmptyString;
	wxPGProperty* property = pg->GetProperty(TYPE_NAME);
	if (!property) return wxEmptyString;
	wxAny value = property->GetValue();
	return wxANY_AS(value, wxString);
}

void IPropertySetting::SplitString2Double(const wxAny& value, double* x, double* y)
{
	wxString val = wxANY_AS(value, wxString);
	size_t gap = val.find_first_of(';');
	wxString sx = val.substr(0, gap),
		sy = val.substr(gap + 1);
	sx.ToDouble(x);
	sy.ToDouble(y);
}

}