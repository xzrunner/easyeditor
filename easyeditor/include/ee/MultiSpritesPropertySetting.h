#ifndef _EASYEDITOR_MULTI_SPRITES_PROPERTY_SETTING_H_
#define _EASYEDITOR_MULTI_SPRITES_PROPERTY_SETTING_H_

#include "PropertySetting.h"

#include <vector>

namespace ee
{

class EditPanelImpl;
class Sprite;
class MultiSpritesPropertyImpl;

class MultiSpritesPropertySetting : public PropertySetting
{
public:
	MultiSpritesPropertySetting(const std::vector<Sprite*>& sprites);
	virtual ~MultiSpritesPropertySetting();

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

protected:
	MultiSpritesPropertyImpl* m_impl;

private:
	EditPanelImpl* m_stage;

	bool m_overall;

}; // MultiSpritesPropertySetting

}

#endif // _EASYEDITOR_MULTI_SPRITES_PROPERTY_SETTING_H_