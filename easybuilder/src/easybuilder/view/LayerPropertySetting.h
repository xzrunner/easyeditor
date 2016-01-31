
#ifndef EBUILDER_LAYER_PROPERTY_SETTING_H
#define EBUILDER_LAYER_PROPERTY_SETTING_H



namespace ebuilder
{
	class LayerItem;
	class LayersPage;

	class LayerPropertySetting : public ee::IPropertySetting
	{
	public:
		LayerPropertySetting(LayerItem* layer, LayersPage* page);

		virtual void updatePanel(ee::PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(ee::PropertySettingPanel* panel);
		virtual void enablePropertyGrid(ee::PropertySettingPanel* panel, bool bEnable);

	private:
		LayerItem* m_layer;

		LayersPage* m_page;

	}; // LayerPropertySetting
}

#endif // EBUILDER_LAYER_PROPERTY_SETTING_H