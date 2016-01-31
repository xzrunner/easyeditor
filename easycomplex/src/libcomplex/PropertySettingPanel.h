#ifndef _EASYCOMPLEX_SYMBOL_PROPERTY_SETTING_H_
#define _EASYCOMPLEX_SYMBOL_PROPERTY_SETTING_H_



namespace ecomplex
{

class PropertySettingPanel : public ee::PropertySettingPanel
{
public:
	PropertySettingPanel(wxWindow* parent);

protected:
	virtual ee::PropertySetting* CreateDefaultProperty() const;

}; // PropertySettingPanel

}

#endif // _EASYCOMPLEX_SYMBOL_PROPERTY_SETTING_H_