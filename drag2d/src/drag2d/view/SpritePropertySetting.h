#ifndef _DRAG2D_SPRITE_PROPERTY_SETTING_H_
#define _DRAG2D_SPRITE_PROPERTY_SETTING_H_

#include "IPropertySetting.h"

namespace d2d
{

class ISprite;
class SpritePropertyImpl;

class SpritePropertySetting : public IPropertySetting
{
public:
	SpritePropertySetting(EditPanel* editPanel, ISprite* sprite);
	virtual ~SpritePropertySetting();

	virtual void updatePanel(PropertySettingPanel* panel);

	virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
	virtual void updatePropertyGrid(PropertySettingPanel* panel);
	virtual void enablePropertyGrid(PropertySettingPanel* panel, bool bEnable);

protected:
	virtual void updateProperties(wxPropertyGrid* pg);
	virtual void initProperties(wxPropertyGrid* pg);

	ISprite* GetSprite();

private:
	SpritePropertyImpl* m_impl;

}; // SpritePropertySetting

}

#endif // _DRAG2D_SPRITE_PROPERTY_SETTING_H_