#ifndef _EASYMODELING_WORLD_PROPERTY_SETTING_H_
#define _EASYMODELING_WORLD_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace emodeling
{

class WorldPropertySetting : public d2d::IPropertySetting
{
public:
	WorldPropertySetting(d2d::EditPanel* editPanel);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

}; // WorldPropertySetting

}

#endif // _EASYMODELING_WORLD_PROPERTY_SETTING_H_