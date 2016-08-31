#include "SymbolPropertySetting.h"
#include "Symbol.h"

#include <ee/StringHelper.h>
#include <ee/panel_msg.h>

namespace ecomplex
{

SymbolPropertySetting::SymbolPropertySetting(Symbol* sym)
	: ee::SymbolPropertySetting(sym)
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
		if (m_sym) {
			SetGroupByNames(wxANY_AS(value, wxString).ToStdString());
		}
	}
	else if (name == wxT("Clipbox") && m_sym) 
	{
		std::vector<std::string> str;
		ee::StringHelper::Split(wxANY_AS(value, wxString).ToStdString(), ";", str);
		if (str.size() == 4) {
			Symbol* c = static_cast<Symbol*>(m_sym);
			sm::rect scissor = c->GetScissor();
			ee::StringHelper::FromString(str[0], scissor.xmin);
			ee::StringHelper::FromString(str[1], scissor.xmax);
			ee::StringHelper::FromString(str[2], scissor.ymin);
			ee::StringHelper::FromString(str[3], scissor.ymax);
			c->SetScissor(scissor);
		}
	}
	else if (name == wxT("Clipbox.xmin") && m_sym)
	{
		Symbol* c = static_cast<Symbol*>(m_sym);
		sm::rect scissor = c->GetScissor();
		scissor.xmin = wxANY_AS(value, int);
		c->SetScissor(scissor);
	}
	else if (name == wxT("Clipbox.xmax") && m_sym)
	{
		Symbol* c = static_cast<Symbol*>(m_sym);
		sm::rect scissor = c->GetScissor();
		scissor.xmax = wxANY_AS(value, int);
		c->SetScissor(scissor);
	}
	else if (name == wxT("Clipbox.ymin") && m_sym)
	{
		Symbol* c = static_cast<Symbol*>(m_sym);
		sm::rect scissor = c->GetScissor();
		scissor.ymin = wxANY_AS(value, int);
		c->SetScissor(scissor);
	}
	else if (name == wxT("Clipbox.ymax") && m_sym)
	{
		Symbol* c = static_cast<Symbol*>(m_sym);
		sm::rect scissor = c->GetScissor();
		scissor.ymax = wxANY_AS(value, int);
		c->SetScissor(scissor);
	}
	else if (name == wxT("Cache") && m_sym)
	{
		Symbol* c = static_cast<Symbol*>(m_sym);
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
	Symbol* complex = static_cast<Symbol*>(m_sym);
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
	Symbol* complex = static_cast<Symbol*>(m_sym);
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

	if (m_sym) {
		pg->GetProperty(wxT("Groups"))->SetValue(GetGroupNames());
	} else {
		pg->GetProperty(wxT("Groups"))->SetValue(wxEmptyString);
	}

	if (m_sym)
	{
		Symbol* c = static_cast<Symbol*>(m_sym);
		const sm::rect& scissor = c->GetScissor();
		pg->SetPropertyValue(wxT("Clipbox.xmin"), (int)scissor.xmin);
		pg->SetPropertyValue(wxT("Clipbox.xmax"), (int)scissor.xmax);
		pg->SetPropertyValue(wxT("Clipbox.ymin"), (int)scissor.ymin);
		pg->SetPropertyValue(wxT("Clipbox.ymax"), (int)scissor.ymax);
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

	if (m_sym) {
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
	if (!m_sym) return;

	Symbol* complex = static_cast<Symbol*>(m_sym);
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