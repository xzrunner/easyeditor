#include "SymbolPropertySetting.h"
#include "Symbol.h"

#include <ee/StringHelper.h>
#include <ee/panel_msg.h>

namespace ecomplex
{

SymbolPropertySetting::SymbolPropertySetting(Symbol* symbol)
	: ee::SymbolPropertySetting(symbol)
{
	m_type = "ComplexSymbol";
}

void SymbolPropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	ee::SymbolPropertySetting::OnPropertyGridChange(name, value);

	if (value.IsNull())
		return;

	bool dirty = true;
	if (name == wxT("Groups"))
	{
		if (m_symbol) {
			SetGroupByNames(wxANY_AS(value, wxString).ToStdString());
		}
	}
	else if (name == wxT("Clipbox") && m_symbol) 
	{
		std::vector<std::string> str;
		ee::StringHelper::Split(wxANY_AS(value, wxString).ToStdString(), ";", str);
		if (str.size() == 4) {
			Symbol* c = static_cast<Symbol*>(m_symbol);
			ee::StringHelper::FromString(str[0], c->m_clipbox.xmin);
			ee::StringHelper::FromString(str[1], c->m_clipbox.xmax);
			ee::StringHelper::FromString(str[2], c->m_clipbox.ymin);
			ee::StringHelper::FromString(str[3], c->m_clipbox.ymax);
		}
	}
	else if (name == wxT("Clipbox.xmin") && m_symbol)
	{
		Symbol* c = static_cast<Symbol*>(m_symbol);
		c->m_clipbox.xmin = wxANY_AS(value, int);
	}
	else if (name == wxT("Clipbox.xmax") && m_symbol)
	{
		Symbol* c = static_cast<Symbol*>(m_symbol);
		c->m_clipbox.xmax = wxANY_AS(value, int);
	}
	else if (name == wxT("Clipbox.ymin") && m_symbol)
	{
		Symbol* c = static_cast<Symbol*>(m_symbol);
		c->m_clipbox.ymin = wxANY_AS(value, int);
	}
	else if (name == wxT("Clipbox.ymax") && m_symbol)
	{
		Symbol* c = static_cast<Symbol*>(m_symbol);
		c->m_clipbox.ymax = wxANY_AS(value, int);
	}
	else if (name == wxT("Cache") && m_symbol)
	{
		Symbol* c = static_cast<Symbol*>(m_symbol);
		c->m_use_render_cache = wxANY_AS(value, bool);
// 		if (ee::Config::Instance()->IsUseDTex()) {
// 			ee::DynamicTexAndFont* dtex = ee::DynamicTexAndFont::Instance();
// 			if (c->m_use_render_cache) {
// 				dtex->InsertSymbol(*c);
// 			} else {
// 				dtex->Remove(c->GetFilepath());
// 			}
// 		}
	}
	else
	{
		dirty = false;
	}

	if (dirty) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

std::string SymbolPropertySetting::GetGroupNames() const
{
	Symbol* complex = static_cast<Symbol*>(m_symbol);
	std::string ret;
	for (int i = 0, n = complex->m_groups.size(); i < n; ++i)
	{
		ret += complex->m_groups[i].name + ";";
	}
	return ret;
}

void SymbolPropertySetting::SetGroupByNames(const std::string& names)
{
	// parser names
	std::set<std::string> set_names;

	std::size_t begin = 0;
	std::size_t end = names.find_first_of(";");
	while (end != std::string::npos) {
		std::string name = names.substr(begin, end - begin);
		if (!name.empty())
			set_names.insert(name);

		begin = end+1;
		end = names.find_first_of(";", begin + 1);
	}
	std::string name = names.substr(begin, end - begin);
	if (!name.empty())
		set_names.insert(name);

	// 
	Symbol* complex = static_cast<Symbol*>(m_symbol);
	std::vector<Symbol::Group>& groups = complex->m_groups;
	// rm symbol's groups
	std::vector<Symbol::Group>::iterator itr 
		= groups.begin();
	for ( ; itr != complex->m_groups.end(); ) 
	{
		const Symbol::Group& group = *itr;
		if (set_names.find(group.name) == set_names.end())
			itr = groups.erase(itr);
		else
			++itr;
	}
	// add symbol's groups
	std::set<std::string>::iterator set_itr = set_names.begin();
	for ( ; set_itr != set_names.end(); ++set_itr)
	{
		int i, n;
		for (i = 0, n = groups.size(); i < n; ++i)
		{
			if (groups[i].name == *set_itr)
				break;
		}
		if (i == groups.size())
		{
			Symbol::Group g;
			g.name = *set_itr;
			groups.push_back(g);
		}
	}
}

void SymbolPropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	ee::SymbolPropertySetting::UpdateProperties(pg);

	if (m_symbol) {
		pg->GetProperty(wxT("Groups"))->SetValue(GetGroupNames());
	} else {
		pg->GetProperty(wxT("Groups"))->SetValue(wxEmptyString);
	}

	if (m_symbol)
	{
		Symbol* c = static_cast<Symbol*>(m_symbol);
		pg->SetPropertyValue(wxT("Clipbox.xmin"), (int)c->m_clipbox.xmin);
		pg->SetPropertyValue(wxT("Clipbox.xmax"), (int)c->m_clipbox.xmax);
		pg->SetPropertyValue(wxT("Clipbox.ymin"), (int)c->m_clipbox.ymin);
		pg->SetPropertyValue(wxT("Clipbox.ymax"), (int)c->m_clipbox.ymax);
		pg->SetPropertyValue(wxT("Cache"), c->m_use_render_cache);
	}
	else
	{
		pg->SetPropertyValue(wxT("Clipbox.xmin"), 0);
		pg->SetPropertyValue(wxT("Clipbox.xmax"), 0);
		pg->SetPropertyValue(wxT("Clipbox.ymin"), 0);
		pg->SetPropertyValue(wxT("Clipbox.ymax"), 0);
		pg->SetPropertyValue(wxT("Cache"), false);			
	}
}

void SymbolPropertySetting::InitProperties(wxPropertyGrid* pg)
{
	ee::SymbolPropertySetting::InitProperties(pg);

	if (m_symbol) {
		pg->Append(new wxStringProperty(wxT("Groups"), wxPG_LABEL, GetGroupNames()));
	} else {
		pg->Append(new wxStringProperty(wxT("Groups"), wxPG_LABEL, wxEmptyString));
	}

	wxPGProperty* cbProp = pg->Append(new wxStringProperty(("Clipbox"), wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(cbProp, new wxIntProperty(wxT("xmin"), wxPG_LABEL, 0));
	pg->AppendIn(cbProp, new wxIntProperty(wxT("xmax"), wxPG_LABEL, 0));
	pg->AppendIn(cbProp, new wxIntProperty(wxT("ymin"), wxPG_LABEL, 0));
	pg->AppendIn(cbProp, new wxIntProperty(wxT("ymax"), wxPG_LABEL, 0));

	pg->Append(new wxBoolProperty(wxT("Cache"), wxPG_LABEL, false));
	pg->SetPropertyAttribute("Cache", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
}

void SymbolPropertySetting::InitEachGroup(wxPropertyGrid* pg)
{
	if (!m_symbol) return;

	Symbol* complex = static_cast<Symbol*>(m_symbol);
	std::vector<Symbol::Group>& groups = complex->m_groups;
	for (int i = 0, n = groups.size(); i < n; ++i)
	{
		const Symbol::Group& g = groups[i];
		wxPGProperty* property = pg->GetProperty(g.name);
		if (property)
		{

		}
		else
		{

		}
	}
}

}