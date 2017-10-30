#ifndef _EASYCOMPLEX_SPRITE_PROPERTY_SETTING_H_
#define _EASYCOMPLEX_SPRITE_PROPERTY_SETTING_H_

#include "Sprite.h"

#include <ee/SpritePropertySetting.h>

namespace ecomplex
{

class SpritePropertySetting : public ee::SpritePropertySetting
{
public:
	SpritePropertySetting(ee::EditPanelImpl* edit_impl, const std::shared_ptr<Sprite>& spr);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

}; // SpritePropertySetting

}

#endif // _EASYCOMPLEX_SPRITE_PROPERTY_SETTING_H_