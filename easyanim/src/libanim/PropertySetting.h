#ifndef _LIBANIM_PROPERTY_SETTING_H_
#define _LIBANIM_PROPERTY_SETTING_H_

#include <ee/SpritePropertySetting.h>

namespace ee { class EditPanelImpl; }

namespace libanim
{

class Sprite;

class PropertySetting : public ee::SpritePropertySetting
{
public:
	PropertySetting(ee::EditPanelImpl* edit_impl, Sprite* spr);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

}; // PropertySetting

}

#endif // _LIBANIM_PROPERTY_SETTING_H_