#ifndef _LIBSHAPE_RECT_PROPERTY_SETTING_H_
#define _LIBSHAPE_RECT_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace libshape
{

class RectShape;

class RectPropertySetting : public d2d::IPropertySetting
{
public:
	RectPropertySetting(d2d::EditPanel* editPanel, RectShape* rect);

	virtual void updatePanel(d2d::PropertySettingPanel* panel);

	virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
	virtual void updatePropertyGrid(d2d::PropertySettingPanel* panel);
	virtual void enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable);

private:
	RectShape* m_rect;

}; // RectPropertySetting

}

#endif // _LIBSHAPE_RECT_PROPERTY_SETTING_H_