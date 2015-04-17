#include "FontPropertySetting.h"

#include "dataset/TidCfg.h"

namespace eui
{

FontPropertySetting::FontPropertySetting(d2d::EditPanel* editPanel, d2d::FontSprite* sprite)
	: d2d::FontPropertySetting(editPanel, sprite)
{
}

void FontPropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	d2d::FontPropertySetting::onPropertyGridChange(name, value);

	if (name == wxT("TextID")) 
	{
		std::string tid = wxANY_AS(value, wxString);

		std::string val = TidCfg::Instance()->Query(tid);
		if (!val.empty()) {
			d2d::FontSprite* sprite = static_cast<d2d::FontSprite*>(m_sprite);
			sprite->SetTextContent(val);
		}
	}
}

}