#ifndef _EASYSCALE9_SPRITE_PROPERTY_SETTING_H_
#define _EASYSCALE9_SPRITE_PROPERTY_SETTING_H_

#include <ee/SpritePropertySetting.h>

namespace escale9
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

#endif // _EASYSCALE9_SPRITE_PROPERTY_SETTING_H_