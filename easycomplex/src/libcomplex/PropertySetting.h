#ifndef COMPLEX_PROPERTY_SETTING_H
#define COMPLEX_PROPERTY_SETTING_H

#include <drag2d.h>

namespace ecomplex
{
	class Symbol;

	class PropertySetting : public d2d::SymbolPropertySetting
	{
	public:
		PropertySetting(d2d::EditPanel* editPanel, Symbol* symbol);

		//virtual void updatePanel(d2d::PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		//virtual void updatePropertyGrid(d2d::PropertySettingPanel* panel);
		virtual void enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable);

	protected:
		virtual void updateProperties(wxPropertyGrid* pg);
		virtual void initProperties(wxPropertyGrid* pg);

	private:
		std::string getGroupNames() const;
		void setGroupByNames(const wxString& names);

		void initEachGroup(wxPropertyGrid* pg);

	}; // PropertySetting	
}

#endif // COMPLEX_PROPERTY_SETTING_H