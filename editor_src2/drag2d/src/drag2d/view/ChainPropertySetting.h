#pragma once

#include "IPropertySetting.h"

namespace d2d
{
	class ChainShape;

	class ChainPropertySetting : public IPropertySetting
	{
	public:
		ChainPropertySetting(EditPanel* editPanel, ChainShape* chain);

		virtual void updatePanel(PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(PropertySettingPanel* panel);
		virtual void enablePropertyGrid(PropertySettingPanel* panel, bool bEnable);

	private:
		ChainShape* m_chain;

	}; // ChainPropertySetting
}

