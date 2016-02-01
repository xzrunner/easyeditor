#ifndef _EASYSHAPE_BEZIER_PROPERTY_SETTING_H_
#define _EASYSHAPE_BEZIER_PROPERTY_SETTING_H_

#include <ee/PropertySetting.h>

namespace ee { class EditPanelImpl; }

namespace eshape
{

class BezierShape;

class BezierPropertySetting : public ee::PropertySetting
{
public:
	BezierPropertySetting(ee::EditPanelImpl* stage, BezierShape* bezier);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	ee::EditPanelImpl* m_stage;

	BezierShape* m_bezier;

}; // BezierPropertySetting

}

#endif // _EASYSHAPE_BEZIER_PROPERTY_SETTING_H_