#pragma once

#include "IPropertySetting.h"

namespace d2d
{
	class PolygonShape;

	class PolygonPropertySetting : public IPropertySetting
	{
	public:
		PolygonPropertySetting(EditPanel* editPanel, PolygonShape* poly);

		virtual void updatePanel(PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(PropertySettingPanel* panel);
		virtual void enablePropertyGrid(PropertySettingPanel* panel, bool bEnable);

	private:
		PolygonShape* m_poly;

	}; // PolygonPropertySetting
}

