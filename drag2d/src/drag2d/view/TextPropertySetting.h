#ifndef _DRAG2D_TEXT_PROPERTY_SETTING_H_
#define _DRAG2D_TEXT_PROPERTY_SETTING_H_

#include "IPropertySetting.h"

namespace d2d
{

class EditPanelImpl;
class TextSprite;

class TextPropertySetting : public IPropertySetting
{
public:
	TextPropertySetting(EditPanelImpl* stage, TextSprite* sprite);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	d2d::EditPanelImpl* m_stage;

	TextSprite* m_sprite;

}; // TextPropertySetting

}

#endif // _DRAG2D_TEXT_PROPERTY_SETTING_H_