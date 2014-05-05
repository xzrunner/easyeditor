#ifndef _LIBSHAPE_POLYGON_PROPERTY_SETTING_H_
#define _LIBSHAPE_POLYGON_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace libshape
{

class PolygonShape;

class PolygonPropertySetting : public d2d::IPropertySetting
{
public:
	PolygonPropertySetting(d2d::EditPanel* editPanel, PolygonShape* poly);

	virtual void updatePanel(d2d::PropertySettingPanel* panel);

	virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
	virtual void updatePropertyGrid(d2d::PropertySettingPanel* panel);
	virtual void enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable);

private:
	PolygonShape* m_poly;

}; // PolygonPropertySetting

}

#endif // _LIBSHAPE_POLYGON_PROPERTY_SETTING_H_