#ifndef _LIBSHAPE_BEZIER_PROPERTY_SETTING_H_
#define _LIBSHAPE_BEZIER_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace libshape
{

class BezierShape;

class BezierPropertySetting : public d2d::IPropertySetting
{
public:
	BezierPropertySetting(d2d::EditPanel* editPanel, BezierShape* bezier);

	virtual void updatePanel(d2d::PropertySettingPanel* panel);

	virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
	virtual void updatePropertyGrid(d2d::PropertySettingPanel* panel);
	virtual void enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable);

private:
	BezierShape* m_bezier;

}; // BezierPropertySetting

}

#endif // _LIBSHAPE_BEZIER_PROPERTY_SETTING_H_