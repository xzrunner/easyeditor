#ifndef _EASYMODELING_BODY_PROPERTY_SETTING_H_
#define _EASYMODELING_BODY_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace libmodeling { class Body; }

namespace emodeling
{

class BodyPropertySetting : public d2d::IPropertySetting
{
public:
	BodyPropertySetting(d2d::EditPanelImpl* stage, d2d::ISprite* sprite);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	d2d::EditPanelImpl* m_stage;

	libmodeling::Body* m_body;

}; // BodyPropertySetting

}

#endif // _EASYMODELING_BODY_PROPERTY_SETTING_H_
