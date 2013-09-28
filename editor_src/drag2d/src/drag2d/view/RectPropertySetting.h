#pragma once

#include "IPropertySetting.h"

namespace d2d
{
	class RectShape;

	class RectPropertySetting : public IPropertySetting
	{
	public:
		RectPropertySetting(EditPanel* editPanel, RectShape* rect);

		virtual void updatePanel(PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(PropertySettingPanel* panel);
		virtual void enablePropertyGrid(PropertySettingPanel* panel, bool bEnable);

	private:
		RectShape* m_rect;

	}; // RectPropertySetting
}

