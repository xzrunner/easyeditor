#include "RandomPasteCMPT.h"
#include "RandomPasteOP.h"

#include "Context.h"

using namespace formation;

RandomPasteCMPT::RandomPasteCMPT(wxWindow* parent, const wxString& name, 
								 d2d::EditPanel* editPanel)
	: d2d::AbstractEditCMPT(parent, name, editPanel)
{
	m_editOP = new RandomPasteOP(editPanel, this);
}

void RandomPasteCMPT::updateByLibrary()
{
	initChoice();
}

wxSizer* RandomPasteCMPT::initLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("random"));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		m_choice = new wxChoice(this, wxID_ANY);
 		Connect(m_choice->GetId(), wxEVT_COMMAND_CHOICE_SELECTED, 
 			wxCommandEventHandler(RandomPasteCMPT::onChoiceChanged));
		initChoice();
		sizer->Add(m_choice, 0, wxALIGN_LEFT);
	}
	sizer->AddSpacer(10);
	{
		wxBoxSizer* numSizer = new wxBoxSizer(wxHORIZONTAL);
		numSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Number: ")));

		m_number = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
			wxSP_ARROW_KEYS, 0, 200, 0);
		Connect(m_number->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, 
			wxSpinEventHandler(RandomPasteCMPT::onNumberChanged));
		numSizer->Add(m_number);

		sizer->Add(numSizer);
	}
	return sizer;
}

void RandomPasteCMPT::initChoice()
{
	std::map<wxString, int> mapNameToNum;

	std::vector<wxString> names;
	Context::Instance()->library->traverse(d2d::GetNameVisitor<d2d::ListItem>(names));

	m_choice->Clear();
	for (size_t i = 0, n = names.size(); i < n; ++i)
	{
		const wxString& name = names[i];

		std::map<wxString, int>::iterator itr = m_mapNameToNum.find(name);
		if (itr != m_mapNameToNum.end())
			mapNameToNum.insert(std::make_pair(name, itr->second));
		else
			mapNameToNum.insert(std::make_pair(name, 0));

		m_choice->Append(name);
	}
	if (!names.empty()) 
		m_choice->SetSelection(0);

	m_mapNameToNum = mapNameToNum;
}

void RandomPasteCMPT::onChoiceChanged(wxCommandEvent& event)
{
	wxString name = m_choice->GetString(m_choice->GetSelection());
	m_number->SetValue(m_mapNameToNum.find(name)->second);
}

void RandomPasteCMPT::onNumberChanged(wxSpinEvent& event)
{
	wxString name = m_choice->GetString(m_choice->GetSelection());
	m_mapNameToNum.find(name)->second = m_number->GetValue();
}