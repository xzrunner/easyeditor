#ifndef _LIBSHAPE_BEZIER_PROPERTY_SETTING_H_
#define _LIBSHAPE_BEZIER_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace libshape
{

class BezierShape;

class BezierPropertySetting : public d2d::IPropertySetting
{
public:
	BezierPropertySetting(d2d::EditPanel* editPanel, BezierShape* bezier);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	BezierShape* m_bezier;

}; // BezierPropertySetting

}

#endif // _LIBSHAPE_BEZIER_PROPERTY_SETTING_H_