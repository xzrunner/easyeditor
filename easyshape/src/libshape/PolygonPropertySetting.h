#ifndef _EASYSHAPE_POLYGON_PROPERTY_SETTING_H_
#define _EASYSHAPE_POLYGON_PROPERTY_SETTING_H_

#include <ee/PropertySetting.h>

namespace ee { class EditPanelImpl; }

namespace eshape
{

class PolygonShape;

class PolygonPropertySetting : public ee::PropertySetting
{
public:
	PolygonPropertySetting(ee::EditPanelImpl* stage, PolygonShape* poly);
	virtual ~PolygonPropertySetting();

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	ee::EditPanelImpl* m_stage;

	PolygonShape* m_poly;

}; // PolygonPropertySetting

}

#endif // _EASYSHAPE_POLYGON_PROPERTY_SETTING_H_