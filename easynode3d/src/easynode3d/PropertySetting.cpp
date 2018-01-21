#include "PropertySetting.h"
#include "PropertySettingPanel.h"

namespace enode3d
{

PropertySetting::PropertySetting(const std::string& type)
	: m_type(type)
{
}

void PropertySetting::UpdateFromPanel(PropertySettingPanel& panel)
{
	auto& pg = panel.GetPropertyGrid();
	if (panel.GetType() == m_type) {
		UpdateProperties(pg);
	} else {
		InitProperties(pg);
	}
}

void PropertySetting::EnablePropertyGrid(PropertySettingPanel& panel, bool enable)
{
	auto& pg = panel.GetPropertyGrid();

	if (panel.GetType() != m_type) {
		InitProperties(pg);
	}

	for (wxPropertyGridIterator itr = pg.GetIterator(wxPG_ITERATE_ALL); !itr.AtEnd(); itr++) {
		wxPGProperty* p = *itr;
		p->Enable(enable);
	}
}

}