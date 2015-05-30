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

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

}; // SpritePropertySetting

}

#endif // _EASYSCALE9_SPRITE_PROPERTY_SETTING_H_