#ifndef _EASYICON_SPRITE_PROPERTY_SETTING_H_
#define _EASYICON_SPRITE_PROPERTY_SETTING_H_

#include <ee/SpritePropertySetting.h>

namespace eicon
{

class Sprite;

class SpritePropertySetting : public ee::SpritePropertySetting
{
public:
	SpritePropertySetting(ee::EditPanelImpl* stage, const std::shared_ptr<Sprite>& spr);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value) override;

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg) override;
	virtual void InitProperties(wxPropertyGrid* pg) override;

}; // SpritePropertySetting

}

#endif // _EASYICON_SPRITE_PROPERTY_SETTING_H_