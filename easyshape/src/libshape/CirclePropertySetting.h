#ifndef _LIBSHAPE_CIRCLE_PROPERTY_SETTING_H_
#define _LIBSHAPE_CIRCLE_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace libshape
{

class CircleShape;

class CirclePropertySetting : public d2d::IPropertySetting
{
public:
	CirclePropertySetting(d2d::EditPanel* editPanel, CircleShape* circle);

	virtual void updatePanel(d2d::PropertySettingPanel* panel);

	virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
	virtual void updatePropertyGrid(d2d::PropertySettingPanel* panel);
	virtual void enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable);

private:
	CircleShape* m_circle;

}; // CirclePropertySetting

}

#endif // _LIBSHAPE_CIRCLE_PROPERTY_SETTING_H_