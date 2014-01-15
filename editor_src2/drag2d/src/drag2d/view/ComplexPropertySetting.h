#pragma once

#include "SymbolPropertySetting.h"

namespace d2d 
{
	class ComplexSymbol;

	class ComplexPropertySetting : public SymbolPropertySetting
	{
	public:
		ComplexPropertySetting(EditPanel* editPanel, ComplexSymbol* symbol);

		//virtual void updatePanel(PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		//virtual void updatePropertyGrid(PropertySettingPanel* panel);
		virtual void enablePropertyGrid(PropertySettingPanel* panel, bool bEnable);

	protected:
		virtual void updateProperties(wxPropertyGrid* pg);
		virtual void initProperties(wxPropertyGrid* pg);

	private:
		std::string getGroupNames() const;
		void setGroupByNames(const wxString& names);

		void initEachGroup(wxPropertyGrid* pg);

	}; // ComplexPropertySetting
}