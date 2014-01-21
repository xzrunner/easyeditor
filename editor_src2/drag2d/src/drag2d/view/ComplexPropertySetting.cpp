#include "ComplexPropertySetting.h"
#include "PropertySettingPanel.h"

#include "dataset/ComplexSymbol.h"
#include "view/EditPanel.h"

#include <set>

namespace d2d
{
ComplexPropertySetting::ComplexPropertySetting(EditPanel* editPanel, ComplexSymbol* symbol)
	: SymbolPropertySetting(editPanel, symbol)
{
}

void ComplexPropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	SymbolPropertySetting::onPropertyGridChange(name, value);

	if (value.IsNull())
		return;

	if (name == wxT("Groups"))
	{
		if (m_symbol)
			setGroupByNames(wxANY_AS(value, wxString));
	}
	else if (name == wxT("Clipbox.xmin") && m_symbol)
	{
		ComplexSymbol* c = static_cast<ComplexSymbol*>(m_symbol);
		c->m_clipbox.xMin = wxANY_AS(value, int);
	}
	else if (name == wxT("Clipbox.xmax") && m_symbol)
	{
		ComplexSymbol* c = static_cast<ComplexSymbol*>(m_symbol);
		c->m_clipbox.xMax = wxANY_AS(value, int);
	}
	else if (name == wxT("Clipbox.ymin") && m_symbol)
	{
		ComplexSymbol* c = static_cast<ComplexSymbol*>(m_symbol);
		c->m_clipbox.yMin = wxANY_AS(value, int);
	}
	else if (name == wxT("Clipbox.ymax") && m_symbol)
	{
		ComplexSymbol* c = static_cast<ComplexSymbol*>(m_symbol);
		c->m_clipbox.yMax = wxANY_AS(value, int);
	}

	m_editPanel->Refresh();
}

void ComplexPropertySetting::enablePropertyGrid(PropertySettingPanel* panel, bool bEnable)
{
	SymbolPropertySetting::enablePropertyGrid(panel, bEnable);
	panel->getPG()->GetProperty(wxT("Groups"))->Enable(bEnable);
	panel->getPG()->GetProperty(wxT("Clipbox"))->Enable(bEnable);
}

std::string ComplexPropertySetting::getGroupNames() const
{
	ComplexSymbol* complex = static_cast<ComplexSymbol*>(m_symbol);
	std::string ret;
	for (int i = 0, n = complex->m_groups.size(); i < n; ++i)
	{
		ret += complex->m_groups[i].name + ";";
	}
	return ret;
}

void ComplexPropertySetting::setGroupByNames(const wxString& names)
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
	ComplexSymbol* complex = static_cast<ComplexSymbol*>(m_symbol);
	std::vector<ComplexSymbol::Group>& groups = complex->m_groups;
	// rm symbol's groups
	std::vector<ComplexSymbol::Group>::iterator itr 
		= groups.begin();
	for ( ; itr != complex->m_groups.end(); ) 
	{
		const ComplexSymbol::Group& group = *itr;
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
			ComplexSymbol::Group g;
			g.name = *set_itr;
			groups.push_back(g);
		}
	}
}

void ComplexPropertySetting::updateProperties(wxPropertyGrid* pg)
{
	SymbolPropertySetting::updateProperties(pg);
	if (m_symbol) 
		pg->GetProperty(wxT("Groups"))->SetValue(getGroupNames());
	else
		pg->GetProperty(wxT("Groups"))->SetValue(wxEmptyString);

	if (m_symbol)
	{
		ComplexSymbol* c = static_cast<ComplexSymbol*>(m_symbol);
		pg->SetPropertyValue(wxT("Clipbox.xmin"), (int)c->m_clipbox.xMin);
		pg->SetPropertyValue(wxT("Clipbox.xmax"), (int)c->m_clipbox.xMax);
		pg->SetPropertyValue(wxT("Clipbox.ymin"), (int)c->m_clipbox.yMin);
		pg->SetPropertyValue(wxT("Clipbox.ymax"), (int)c->m_clipbox.yMax);
	}
	else
	{
		pg->SetPropertyValue(wxT("Clipbox.xmin"), 0);
		pg->SetPropertyValue(wxT("Clipbox.xmax"), 0);
		pg->SetPropertyValue(wxT("Clipbox.ymin"), 0);
		pg->SetPropertyValue(wxT("Clipbox.ymax"), 0);
	}
}

void ComplexPropertySetting::initProperties(wxPropertyGrid* pg)
{
	SymbolPropertySetting::initProperties(pg);
	if (m_symbol)
		pg->Append(new wxStringProperty(wxT("Groups"), wxPG_LABEL, getGroupNames()));
	else
		pg->Append(new wxStringProperty(wxT("Groups"), wxPG_LABEL, wxEmptyString));

	wxPGProperty* cbProp = pg->Append(new wxStringProperty(("Clipbox"), wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(cbProp, new wxIntProperty(wxT("xmin"), wxPG_LABEL, 0));
	pg->AppendIn(cbProp, new wxIntProperty(wxT("xmax"), wxPG_LABEL, 0));
	pg->AppendIn(cbProp, new wxIntProperty(wxT("ymin"), wxPG_LABEL, 0));
	pg->AppendIn(cbProp, new wxIntProperty(wxT("ymax"), wxPG_LABEL, 0));
}

void ComplexPropertySetting::initEachGroup(wxPropertyGrid* pg)
{
	if (!m_symbol) return;

	ComplexSymbol* complex = static_cast<ComplexSymbol*>(m_symbol);
	std::vector<ComplexSymbol::Group>& groups = complex->m_groups;
	for (int i = 0, n = groups.size(); i < n; ++i)
	{
		const ComplexSymbol::Group& g = groups[i];
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