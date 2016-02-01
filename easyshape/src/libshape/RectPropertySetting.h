#ifndef _EASYSHAPE_RECT_PROPERTY_SETTING_H_
#define _EASYSHAPE_RECT_PROPERTY_SETTING_H_

#include <ee/PropertySetting.h>

namespace ee { class EditPanelImpl; }

namespace eshape
{

class RectShape;

class RectPropertySetting : public ee::PropertySetting
{
public:
	RectPropertySetting(ee::EditPanelImpl* stage, RectShape* rect);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	ee::EditPanelImpl* m_stage;

	RectShape* m_rect;

}; // RectPropertySetting

}

#endif // _EASYSHAPE_RECT_PROPERTY_SETTING_H_