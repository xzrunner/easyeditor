#ifndef _EASYUI_FONT_PROPERTY_SETTING_H_
#define _EASYUI_FONT_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace eui
{

class FontPropertySetting : public d2d::FontPropertySetting
{
public:
	FontPropertySetting(d2d::EditPanel* editPanel, d2d::FontSprite* sprite);

	virtual void onPropertyGridChange(const wxString& name, const wxAny& value);

}; // FontPropertySetting

}

#endif // _EASYUI_FONT_PROPERTY_SETTING_H_