#ifndef _EASYANIM_FRAME_PROPERTY_SETTING_H_
#define _EASYANIM_FRAME_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace eanim
{

class KeyFrame;

class KeyFramePropertySetting : public d2d::IPropertySetting
{
public:
	KeyFramePropertySetting(d2d::EditPanel* editpanel, KeyFrame* frame);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	KeyFrame* m_frame;

}; // KeyFramePropertySetting

}

#endif // _EASYANIM_FRAME_PROPERTY_SETTING_H_