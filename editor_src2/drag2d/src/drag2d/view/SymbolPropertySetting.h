#pragma once

#include "IPropertySetting.h"

namespace d2d
{
	class ISymbol;

	class SymbolPropertySetting : public IPropertySetting
	{
	public:
		SymbolPropertySetting(EditPanel* editPanel, ISymbol* symbol);
		SymbolPropertySetting(EditPanel* editPanel, std::string* name);

		virtual void updatePanel(PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(PropertySettingPanel* panel);
		virtual void enablePropertyGrid(PropertySettingPanel* panel, bool bEnable);

	protected:
		virtual void updateProperties(wxPropertyGrid* pg);
		virtual void initProperties(wxPropertyGrid* pg);

	protected:
		ISymbol* m_symbol;

		std::string* m_name;

	}; // SymbolPropertySetting
}

