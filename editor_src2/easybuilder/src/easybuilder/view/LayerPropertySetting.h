
#ifndef EBUILDER_LAYER_PROPERTY_SETTING_H
#define EBUILDER_LAYER_PROPERTY_SETTING_H

#include <drag2d.h>

namespace ebuilder
{
	class LayerItem;
	class LayersPage;

	class LayerPropertySetting : public d2d::IPropertySetting
	{
	public:
		LayerPropertySetting(LayerItem* layer, LayersPage* page);

		virtual void updatePanel(d2d::PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(d2d::PropertySettingPanel* panel);
		virtual void enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable);

	private:
		LayerItem* m_layer;

		LayersPage* m_page;

	}; // LayerPropertySetting
}

#endif // EBUILDER_LAYER_PROPERTY_SETTING_H