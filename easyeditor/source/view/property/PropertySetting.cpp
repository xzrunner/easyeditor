#include "PropertySetting.h"
#include "PropertySettingPanel.h"
#include "StringHelper.h"

namespace ee
{

const std::string PropertySetting::TYPE_NAME = "Type";

PropertySetting::PropertySetting(const std::string& type)
	: m_type(type)
{
}

void PropertySetting::UpdateFromPanel(PropertySettingPanel* panel)
{
	wxPropertyGrid* pg = panel->GetPropertyGrid();
	if (panel->GetType() == m_type) {
		UpdateProperties(pg);
	} else {
		InitProperties(pg);
	}
}

void PropertySetting::EnablePropertyGrid(PropertySettingPanel* panel, bool enable)
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

std::string PropertySetting::GetPGType(wxPropertyGrid* pg) const
{
	if (!pg) {
		return "";
	}

	wxPGProperty* property = pg->GetProperty(TYPE_NAME);
	if (!property) {
		return "";
	}

	wxAny value = property->GetValue();
	return wxANY_AS(value, wxString).ToStdString();
}

void PropertySetting::SplitString2Double(const wxAny& value, double* x, double* y)
{
	std::vector<std::string> str;
	StringHelper::Split(wxANY_AS(value, wxString).ToStdString(), ";", str);
	if (str.size() == 2) {
		StringHelper::FromString(str[0], *x);
		StringHelper::FromString(str[1], *y);
	}
}

}