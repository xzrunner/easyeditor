#ifndef _EASYCOMPLEX_PROPERTY_SETTING_H_
#define _EASYCOMPLEX_PROPERTY_SETTING_H_



namespace ecomplex
{

class Symbol;

class SymbolPropertySetting : public ee::SymbolPropertySetting
{
public:
	SymbolPropertySetting(Symbol* symbol);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	std::string GetGroupNames() const;
	void SetGroupByNames(const wxString& names);

	void InitEachGroup(wxPropertyGrid* pg);

}; // SymbolPropertySetting	

}

#endif // _EASYCOMPLEX_PROPERTY_SETTING_H_