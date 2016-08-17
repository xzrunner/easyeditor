#ifndef _EASYSCALE9_SPRITE_PROPERTY_SETTING_H_
#define _EASYSCALE9_SPRITE_PROPERTY_SETTING_H_

#include <ee/SpritePropertySetting.h>

namespace escale9
{

class Sprite;

class SpritePropertySetting : public ee::SpritePropertySetting
{
public:
	SpritePropertySetting(ee::EditPanelImpl* stage, Sprite* spr);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

}; // SpritePropertySetting

}

#endif // _EASYSCALE9_SPRITE_PROPERTY_SETTING_H_