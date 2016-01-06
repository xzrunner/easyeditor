#ifndef _EASYPARTICLE3D_PROPERTY_SETTING_PANEL_H_
#define _EASYPARTICLE3D_PROPERTY_SETTING_PANEL_H_

#include <drag2d.h>

namespace eparticle3d
{

class StagePanel;

class PropertySettingPanel : public d2d::PropertySettingPanel
{
public:
	PropertySettingPanel(wxWindow* parent);

	void SetStagePanel(StagePanel* stage) { m_stage = stage; }

protected:
	virtual d2d::IPropertySetting* CreateDefaultProperty() const;

private:
	StagePanel* m_stage;

}; // PropertySettingPanel

}

#endif // _EASYPARTICLE3D_PROPERTY_SETTING_PANEL_H_