#include "FontBlankPropertySetting.h"
#include "TidCfg.h"

namespace eui
{

FontBlankPropertySetting::FontBlankPropertySetting(d2d::EditPanelImpl* stage, d2d::FontBlankSprite* sprite)
	: d2d::FontBlankPropertySetting(stage, sprite)
{
}

void FontBlankPropertySetting::OnPropertyGridChange(const wxString& name, const wxAny& value)
{
	d2d::FontBlankPropertySetting::OnPropertyGridChange(name, value);

	if (name == wxT("TextID")) 
	{
		std::string tid = wxANY_AS(value, wxString);

		std::string val = TidCfg::Instance()->Query(tid);
		if (!val.empty()) {
			d2d::FontBlankSprite* sprite = static_cast<d2d::FontBlankSprite*>(GetSprite());
			sprite->SetTextContent(val);
		}
	}
}

}