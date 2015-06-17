#ifndef _EASYCOMPLEX_PROPERTY_SETTING_H_
#define _EASYCOMPLEX_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace ecomplex
{

class Symbol;

class SymbolPropertySetting : public d2d::SymbolPropertySetting
{
public:
	SymbolPropertySetting(d2d::EditPanel* stage, Symbol* symbol);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	std::string GetGroupNames() const;
	void SetGroupByNames(const wxString& names);

	void InitEachGroup(wxPropertyGrid* pg);

private:
	d2d::EditPanel* m_stage;

}; // SymbolPropertySetting	

}

#endif // _EASYCOMPLEX_PROPERTY_SETTING_H_