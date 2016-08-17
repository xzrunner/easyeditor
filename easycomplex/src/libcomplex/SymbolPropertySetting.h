#ifndef _EASY_EASYCOMPLEX_PROPERTY_SETTING_H_
#define _EASY_EASYCOMPLEX_PROPERTY_SETTING_H_

#include <ee/SymbolPropertySetting.h>

namespace ecomplex
{

class Symbol;

class SymbolPropertySetting : public ee::SymbolPropertySetting
{
public:
	SymbolPropertySetting(Symbol* sym);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	std::string GetGroupNames() const;
	void SetGroupByNames(const std::string& names);

	void InitEachGroup(wxPropertyGrid* pg);

}; // SymbolPropertySetting	

}

#endif // _EASY_EASYCOMPLEX_PROPERTY_SETTING_H_