#ifndef _DRAG2D_MULTI_SPRITES_PROPERTY_SETTING_H_
#define _DRAG2D_MULTI_SPRITES_PROPERTY_SETTING_H_

#include "IPropertySetting.h"

#include <vector>

namespace d2d
{

class EditPanelImpl;
class Sprite;
class MultiSpritesPropertyImpl;

class MultiSpritesPropertySetting : public IPropertySetting
{
public:
	MultiSpritesPropertySetting(const std::vector<Sprite*>& sprites);
	virtual ~MultiSpritesPropertySetting();

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

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

#endif // _DRAG2D_MULTI_SPRITES_PROPERTY_SETTING_H_