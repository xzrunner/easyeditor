#ifndef _EASYMODELING_WORLD_PROPERTY_SETTING_H_
#define _EASYMODELING_WORLD_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace emodeling
{

class WorldPropertySetting : public d2d::IPropertySetting
{
public:
	WorldPropertySetting(d2d::EditPanelImpl* stage);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	d2d::EditPanelImpl* m_stage;

}; // WorldPropertySetting

}

#endif // _EASYMODELING_WORLD_PROPERTY_SETTING_H_