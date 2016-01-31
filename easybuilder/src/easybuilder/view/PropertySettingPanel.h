
#ifndef EBUILDER_PROPERTY_SETTING_PANEL_H
#define EBUILDER_PROPERTY_SETTING_PANEL_H



namespace ebuilder
{
	class PropertySettingPanel : public ee::PropertySettingPanel
	{
	public:
		PropertySettingPanel(wxWindow* parent);

	private:
		void initLayout();

		void initTitle(wxSizer* sizer);
		void initPropertyGrid(wxSizer* sizer);

	}; // PropertySettingPanel
}

#endif // EBUILDER_PROPERTY_SETTING_PANEL_H
