#ifndef _EASYUI_FONT_PROPERTY_SETTING_H_
#define _EASYUI_FONT_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace eui
{

class FontBlankPropertySetting : public d2d::FontBlankPropertySetting
{
public:
	FontBlankPropertySetting(d2d::EditPanelImpl* stage, d2d::FontBlankSprite* sprite);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

}; // FontBlankPropertySetting

}

#endif // _EASYUI_FONT_PROPERTY_SETTING_H_