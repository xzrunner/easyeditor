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

	//
	// interface Observer
	//
	virtual void Notify(int sj_id, void* ud);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

	ISprite* GetSprite();

protected:
	d2d::EditPanelImpl* m_stage;

private:
	SpritePropertyImpl* m_impl;

	wxPropertyGrid* m_pg;

}; // SpritePropertySetting

}

#endif // _DRAG2D_SPRITE_PROPERTY_SETTING_H_