#pragma once

#include "IPropertySetting.h"

namespace d2d
{
	class BezierShape;

	class BezierPropertySetting : public IPropertySetting
	{
	public:
		BezierPropertySetting(EditPanel* editPanel, BezierShape* bezier);

		virtual void updatePanel(PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(PropertySettingPanel* panel);
		virtual void enablePropertyGrid(PropertySettingPanel* panel, bool bEnable);

	private:
		BezierShape* m_bezier;

	}; // BezierPropertySetting
}

