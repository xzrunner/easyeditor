#ifndef _EASYSHAPE_CIRCLE_PROPERTY_SETTING_H_
#define _EASYSHAPE_CIRCLE_PROPERTY_SETTING_H_

#include <ee/PropertySetting.h>

namespace ee { class EditPanelImpl; }

namespace eshape
{

class CircleShape;

class CirclePropertySetting : public ee::PropertySetting
{
public:
	CirclePropertySetting(ee::EditPanelImpl* stage, CircleShape* circle);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	ee::EditPanelImpl* m_stage;

	CircleShape* m_circle;

}; // CirclePropertySetting

}

#endif // _EASYSHAPE_CIRCLE_PROPERTY_SETTING_H_