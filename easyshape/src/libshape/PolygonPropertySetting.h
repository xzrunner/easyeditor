#ifndef _EASYSHAPE_POLYGON_PROPERTY_SETTING_H_
#define _EASYSHAPE_POLYGON_PROPERTY_SETTING_H_

#include <ee/PropertySetting.h>

#include <memory>

namespace ee { class EditPanelImpl; }

namespace eshape
{

class PolygonShape;

class PolygonPropertySetting : public ee::PropertySetting
{
public:
	PolygonPropertySetting(ee::EditPanelImpl* stage, 
		const std::shared_ptr<PolygonShape>& poly);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value) override;

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg) override;
	virtual void InitProperties(wxPropertyGrid* pg) override;

private:
	ee::EditPanelImpl* m_stage;

	std::shared_ptr<PolygonShape> m_poly;

}; // PolygonPropertySetting

}

#endif // _EASYSHAPE_POLYGON_PROPERTY_SETTING_H_