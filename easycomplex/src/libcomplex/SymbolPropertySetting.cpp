#include "SymbolPropertySetting.h"
#include "Symbol.h"

namespace ecomplex
{

SymbolPropertySetting::SymbolPropertySetting(d2d::EditPanel* stage, Symbol* symbol)
	: d2d::SymbolPropertySetting(stage, symbol)
	, m_stage(stage)
{
	m_type = "ComplexSymbol";
}

void SymbolPropertySetting::OnPropertyGridChange(const wxString& name, const wxAny& value)
{
	d2d::SymbolPropertySetting::OnPropertyGridChange(name, value);

	if (value.IsNull())
		return;

	if (name == wxT("Groups"))
	{
		if (m_symbol) {
			SetGroupByNames(wxANY_AS(value, wxString));
		}
	}
	else if (name == wxT("Clipbox.xmin") && m_symbol)
	{
		Symbol* c = static_cast<Symbol*>(m_symbol);
		c->m_clipbox.xMin = wxANY_AS(value, int);
	}
	else if (name == wxT("Clipbox.xmax") && m_symbol)
	{
		Symbol* c = static_cast<Symbol*>(m_symbol);
		c->m_clipbox.xMax = wxANY_AS(value, int);
	}
	else if (name == wxT("Clipbox.ymin") && m_symbol)
	{
		Symbol* c = static_cast<Symbol*>(m_symbol);
		c->m_clipbox.yMin = wxANY_AS(value, int);
	}
	else if (name == wxT("Clipbox.ymax") && m_symbol)
	{
		Symbol* c = static_cast<Symbol*>(m_symbol);
		c->m_clipbox.yMax = wxANY_AS(value, int);
	}
	else if (name == wxT("Cache") && m_symbol)
	{
		Symbol* c = static_cast<Symbol*>(m_symbol);
		c->m_use_render_cache = wxANY_AS(value, bool);
		if (d2d::Config::Instance()->IsUseDTex()) {
			d2d::DynamicTexAndFont* dtex = d2d::DynamicTexAndFont::Instance();
			if (c->m_use_render_cache) {
				dtex->InsertSymbol(*c);
			} else {
				dtex->Remove(c->GetFilepath());
			}
		}
	}

	m_stage->RefreshStage();
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

void SymbolPropertySetting::SetGroupByNames(const wxString& names)
{
	std::string str = names.ToStdString();

	// parser names
	std::set<std::string> set_names;

	std::size_t begin = 0;
	std::size_t end = str.find_first_of(";");
	while (end != std::string::npos) {
		std::string name = str.substr(begin, end - begin);
		if (!name.empty())
			set_names.insert(name);

		begin = end+1;
		end = str.find_first_of(";", begin + 1);
	}
	std::string name = str.substr(begin, end - begin);
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
		int i;
		for (i = 0; i < groups.size(); ++i)
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
	d2d::SymbolPropertySetting::UpdateProperties(pg);

	if (m_symbol) 
		pg->GetProperty(wxT("Groups"))->SetValue(GetGroupNames());
	else
		pg->GetProperty(wxT("Groups"))->SetValue(wxEmptyString);

	if (m_symbol)
	{
		Symbol* c = static_cast<Symbol*>(m_symbol);
		pg->SetPropertyValue(wxT("Clipbox.xmin"), (int)c->m_clipbox.xMin);
		pg->SetPropertyValue(wxT("Clipbox.xmax"), (int)c->m_clipbox.xMax);
		pg->SetPropertyValue(wxT("Clipbox.ymin"), (int)c->m_clipbox.yMin);
		pg->SetPropertyValue(wxT("Clipbox.ymax"), (int)c->m_clipbox.yMax);
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
	d2d::SymbolPropertySetting::InitProperties(pg);

	if (m_symbol)
		pg->Append(new wxStringProperty(wxT("Groups"), wxPG_LABEL, GetGroupNames()));
	else
		pg->Append(new wxStringProperty(wxT("Groups"), wxPG_LABEL, wxEmptyString));

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