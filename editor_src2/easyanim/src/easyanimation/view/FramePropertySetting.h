#pragma once

#include <drag2d.h>

namespace eanim
{
	class KeyFrame;

	class FramePropertySetting : public d2d::IPropertySetting
	{
	public:
		FramePropertySetting(d2d::EditPanel* editpanel, KeyFrame* frame);

		virtual void updatePanel(d2d::PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(d2d::PropertySettingPanel* panel);
		virtual void enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable);

	private:
		KeyFrame* m_frame;

	}; // FramePropertySetting
}

