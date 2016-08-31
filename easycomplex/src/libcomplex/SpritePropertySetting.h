#ifndef _EASYCOMPLEX_SPRITE_PROPERTY_SETTING_H_
#define _EASYCOMPLEX_SPRITE_PROPERTY_SETTING_H_

#include <ee/SpritePropertySetting.h>

namespace ecomplex
{

class Sprite;

class SpritePropertySetting : public ee::SpritePropertySetting
{
public:
	SpritePropertySetting(ee::EditPanelImpl* edit_impl, Sprite* spr);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

}; // SpritePropertySetting

}

#endif // _EASYCOMPLEX_SPRITE_PROPERTY_SETTING_H_