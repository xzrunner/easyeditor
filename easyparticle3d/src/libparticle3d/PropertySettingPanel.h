#ifndef _EASYPARTICLE3D_PROPERTY_SETTING_PANEL_H_
#define _EASYPARTICLE3D_PROPERTY_SETTING_PANEL_H_



namespace eparticle3d
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

#endif // _EASYPARTICLE3D_PROPERTY_SETTING_PANEL_H_