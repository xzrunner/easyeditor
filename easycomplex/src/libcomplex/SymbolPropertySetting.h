#ifndef _EASYCOMPLEX_SYMBOL_PROPERTY_SETTING_H_
#define _EASYCOMPLEX_SYMBOL_PROPERTY_SETTING_H_

#include <ee/SymbolPropertySetting.h>

namespace ecomplex
{

class Symbol;

class SymbolPropertySetting : public ee::SymbolPropertySetting
{
public:
	SymbolPropertySetting(const std::shared_ptr<Symbol>& sym);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value) override;

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg) override;
	virtual void InitProperties(wxPropertyGrid* pg) override;

private:
	std::string GetGroupNames() const;
	void SetGroupByNames(const std::string& names);

	void InitEachGroup(wxPropertyGrid* pg);

}; // SymbolPropertySetting	

}

#endif // _EASYCOMPLEX_SYMBOL_PROPERTY_SETTING_H_