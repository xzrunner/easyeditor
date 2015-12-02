#ifndef _DRAG2D_SPRITE_PROPERTY_SETTING_H_
#define _DRAG2D_SPRITE_PROPERTY_SETTING_H_

#include "IPropertySetting.h"

#include "message/Observer.h"

namespace d2d
{

class EditPanelImpl;
class ISprite;
class SpritePropertyImpl;

class SpritePropertySetting : public IPropertySetting, public Observer
{
public:
	SpritePropertySetting(EditPanelImpl* stage, ISprite* sprite);
	virtual ~SpritePropertySetting();

	//
	// interface IPropertySetting
	//
	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	//
	// interface IPropertySetting
	//
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

	//
	// interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

	ISprite* GetSprite();

private:
	SpritePropertyImpl* m_impl;

	wxPropertyGrid* m_pg;

}; // SpritePropertySetting

}

#endif // _DRAG2D_SPRITE_PROPERTY_SETTING_H_