#ifndef _LIBSHAPE_POLYGON_PROPERTY_SETTING_H_
#define _LIBSHAPE_POLYGON_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace libshape
{

class PolygonShape;

class PolygonPropertySetting : public d2d::IPropertySetting
{
public:
	PolygonPropertySetting(d2d::EditPanel* stage, PolygonShape* poly);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	d2d::EditPanel* m_stage;

	PolygonShape* m_poly;

}; // PolygonPropertySetting

}

#endif // _LIBSHAPE_POLYGON_PROPERTY_SETTING_H_