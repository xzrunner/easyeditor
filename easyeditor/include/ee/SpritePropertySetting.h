#ifndef _EASYEDITOR_SPRITE_PROPERTY_SETTING_H_
#define _EASYEDITOR_SPRITE_PROPERTY_SETTING_H_

#include "PropertySetting.h"
#include "Observer.h"
#include "SprFilterProperty.h"
#include "Sprite.h"

namespace ee
{

class EditPanelImpl;
class SpritePropertyImpl;

class SpritePropertySetting : public PropertySetting, public Observer
{
public:
	SpritePropertySetting(EditPanelImpl* stage, const SprPtr& spr);
	virtual ~SpritePropertySetting();

	//
	// interface PropertySetting
	//
	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value);

protected:
	//
	// interface PropertySetting
	//
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

	//
	// interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

	SprPtr GetSprite();

private:
	static void SetColMul(const ee::SprPtr& spr, const std::string& val);
	static void SetColAdd(const ee::SprPtr& spr, const std::string& val);
	static void SetColAlpha(const ee::SprPtr& spr, const std::string& val);
	static void SetColOverlap(const ee::SprPtr& spr, const std::string& val);

private:
	SpritePropertyImpl* m_impl;

	wxPropertyGrid* m_pg;

	SprFilterProperty m_spr_filter_prop;

}; // SpritePropertySetting

}

#endif // _EASYEDITOR_SPRITE_PROPERTY_SETTING_H_