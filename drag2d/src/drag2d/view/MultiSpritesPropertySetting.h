#ifndef _DRAG2D_MULTI_SPRITES_PROPERTY_SETTING_H_
#define _DRAG2D_MULTI_SPRITES_PROPERTY_SETTING_H_

#include "IPropertySetting.h"

#include <vector>

namespace d2d
{

class ISprite;
class MultiSpritesPropertyImpl;

class MultiSpritesPropertySetting : public IPropertySetting
{
public:
	MultiSpritesPropertySetting(EditPanel* editPanel, 
		const std::vector<ISprite*>& sprites);
	virtual ~MultiSpritesPropertySetting();

	virtual void updatePanel(PropertySettingPanel* panel);

	virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
	virtual void updatePropertyGrid(PropertySettingPanel* panel);
	virtual void enablePropertyGrid(PropertySettingPanel* panel, bool bEnable);

protected:
	virtual void updateProperties(wxPropertyGrid* pg);
	virtual void initProperties(wxPropertyGrid* pg);

protected:
	MultiSpritesPropertyImpl* m_impl;

private:
	bool m_overall;

}; // MultiSpritesPropertySetting

}

#endif // _DRAG2D_MULTI_SPRITES_PROPERTY_SETTING_H_