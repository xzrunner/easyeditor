#ifndef _EASYANIM_FRAME_PROPERTY_SETTING_H_
#define _EASYANIM_FRAME_PROPERTY_SETTING_H_

#include <ee/PropertySetting.h>

namespace eanim
{

class KeyFrame;

class KeyFramePropertySetting : public ee::PropertySetting
{
public:
	KeyFramePropertySetting(KeyFrame* frame);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	KeyFrame* m_frame;

}; // KeyFramePropertySetting

}

#endif // _EASYANIM_FRAME_PROPERTY_SETTING_H_