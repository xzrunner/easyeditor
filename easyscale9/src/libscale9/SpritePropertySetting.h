#ifndef _EASYSCALE9_SPRITE_PROPERTY_SETTING_H_
#define _EASYSCALE9_SPRITE_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace escale9
{

class Sprite;

class SpritePropertySetting : public d2d::SpritePropertySetting
{
public:
	SpritePropertySetting(d2d::EditPanel* editPanel, Sprite* sprite);

	virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
	virtual void enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable);

protected:
	virtual void updateProperties(wxPropertyGrid* pg);
	virtual void initProperties(wxPropertyGrid* pg);

private:
	

}; // SpritePropertySetting

}

#endif // _EASYSCALE9_SPRITE_PROPERTY_SETTING_H_