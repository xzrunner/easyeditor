#include "FontPropertySetting.h"
#include "TidCfg.h"

namespace eui
{

FontPropertySetting::FontPropertySetting(d2d::EditPanelImpl* stage, d2d::FontSprite* sprite)
	: d2d::FontPropertySetting(stage, sprite)
{
}

void FontPropertySetting::OnPropertyGridChange(const wxString& name, const wxAny& value)
{
	d2d::FontPropertySetting::OnPropertyGridChange(name, value);

	if (name == wxT("TextID")) 
	{
		std::string tid = wxANY_AS(value, wxString);

		std::string val = TidCfg::Instance()->Query(tid);
		if (!val.empty()) {
			d2d::FontSprite* sprite = static_cast<d2d::FontSprite*>(GetSprite());
			sprite->SetTextContent(val);
		}
	}
}

}