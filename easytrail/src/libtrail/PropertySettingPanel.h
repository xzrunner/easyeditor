#ifndef _EASYTRAIL_PROPERTY_SETTING_PANEL_H_
#define _EASYTRAIL_PROPERTY_SETTING_PANEL_H_

#include <ee/PropertySettingPanel.h>

namespace etrail
{

class StagePanel;

class PropertySettingPanel : public ee::PropertySettingPanel
{
public:
	PropertySettingPanel(wxWindow* parent);

	void SetStagePanel(StagePanel* stage) { m_stage = stage; }

protected:
	virtual ee::PropertySetting* CreateDefaultProperty() const;

private:
	StagePanel* m_stage;

}; // PropertySettingPanel

}

#endif // _EASYTRAIL_PROPERTY_SETTING_PANEL_H_