#ifndef _EASYMODELING_WORLD_PROPERTY_SETTING_H_
#define _EASYMODELING_WORLD_PROPERTY_SETTING_H_

#include <ee/PropertySetting.h>

namespace ee { class EditPanelImpl; }

namespace emodeling
{

class WorldPropertySetting : public ee::PropertySetting
{
public:
	WorldPropertySetting(ee::EditPanelImpl* stage);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value) override;

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg) override;
	virtual void InitProperties(wxPropertyGrid* pg) override;

private:
	ee::EditPanelImpl* m_stage;

}; // WorldPropertySetting

}

#endif // _EASYMODELING_WORLD_PROPERTY_SETTING_H_