#ifndef _LIBSHAPE_RECT_PROPERTY_SETTING_H_
#define _LIBSHAPE_RECT_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace libshape
{

class RectShape;

class RectPropertySetting : public d2d::IPropertySetting
{
public:
	RectPropertySetting(d2d::EditPanel* editPanel, RectShape* rect);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	RectShape* m_rect;

}; // RectPropertySetting

}

#endif // _LIBSHAPE_RECT_PROPERTY_SETTING_H_