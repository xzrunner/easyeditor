#ifndef _LIBSHAPE_CIRCLE_PROPERTY_SETTING_H_
#define _LIBSHAPE_CIRCLE_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace libshape
{

class CircleShape;

class CirclePropertySetting : public d2d::IPropertySetting
{
public:
	CirclePropertySetting(d2d::EditPanelImpl* stage, CircleShape* circle);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	d2d::EditPanelImpl* m_stage;

	CircleShape* m_circle;

}; // CirclePropertySetting

}

#endif // _LIBSHAPE_CIRCLE_PROPERTY_SETTING_H_