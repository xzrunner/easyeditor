#ifndef _EASYMODELING_BODY_PROPERTY_SETTING_H_
#define _EASYMODELING_BODY_PROPERTY_SETTING_H_

#include <ee/PropertySetting.h>

namespace ee { class EditPanelImpl; class Sprite; }

namespace emodeling
{

class Body;

class BodyPropertySetting : public ee::PropertySetting
{
public:
	BodyPropertySetting(ee::EditPanelImpl* stage, ee::Sprite* spr);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	ee::EditPanelImpl* m_stage;

	Body* m_body;

}; // BodyPropertySetting

}

#endif // _EASYMODELING_BODY_PROPERTY_SETTING_H_
