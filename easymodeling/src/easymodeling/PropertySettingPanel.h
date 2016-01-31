#ifndef _EASYMODELING_PROPERTY_SETTING_PANEL_H_
#define _EASYMODELING_PROPERTY_SETTING_PANEL_H_



namespace emodeling
{

class PropertySettingPanel : public ee::PropertySettingPanel
{
public:
	PropertySettingPanel(wxWindow* parent);

protected:
	virtual void OnSpriteSelected(ee::Sprite* spr, bool clear);

}; // PropertySettingPanel
 

}

#endif // _EASYMODELING_PROPERTY_SETTING_PANEL_H_