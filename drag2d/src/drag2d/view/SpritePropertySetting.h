#ifndef _DRAG2D_SPRITE_PROPERTY_SETTING_H_
#define _DRAG2D_SPRITE_PROPERTY_SETTING_H_

#include "IPropertySetting.h"

namespace d2d
{

class EditPanel;
class ISprite;
class SpritePropertyImpl;

class SpritePropertySetting : public IPropertySetting
{
public:
	SpritePropertySetting(EditPanel* stage, ISprite* sprite);
	virtual ~SpritePropertySetting();

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

	ISprite* GetSprite();

protected:
	d2d::EditPanel* m_stage;

private:
	SpritePropertyImpl* m_impl;

}; // SpritePropertySetting

}

#endif // _DRAG2D_SPRITE_PROPERTY_SETTING_H_