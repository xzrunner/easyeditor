#pragma once

#include "IPropertySetting.h"

namespace d2d
{
	class TextSprite;

	class TextPropertySetting : public IPropertySetting
	{
	public:
		TextPropertySetting(EditPanel* editPanel, TextSprite* sprite);

		virtual void updatePanel(PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(PropertySettingPanel* panel);
		virtual void enablePropertyGrid(PropertySettingPanel* panel, bool bEnable);

	private:
		TextSprite* m_sprite;

	}; // TextPropertySetting
}

