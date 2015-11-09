#ifndef _EASYTEXT_PROPERTY_SETTING_H_
#define _EASYTEXT_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace etext
{

class Sprite;

class PropertySetting : public d2d::SpritePropertySetting
{
public:
	PropertySetting(d2d::EditPanelImpl* edit_impl, Sprite* sprite);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

}; // PropertySetting

}

#endif // _EASYTEXT_PROPERTY_SETTING_H_