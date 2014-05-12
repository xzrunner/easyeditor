
#include "ChangeImagePanel.h"
#include "ChangeImage.h"

#include "view/Context.h"
#include "view/StagePanel.h"
#include "output/DesignerPage.h"

using namespace ebuilder;

ChangeImagePanel::ChangeImagePanel(wxWindow* parent,
								   d2d::LibraryList* mediaList, 
								   ChangeImage* behavior)
	: BehaviorPanel(parent, CHANGE_IMAGE_NAME)
	, m_list(mediaList)
	, m_behavior(behavior)
{
}

void ChangeImagePanel::update(const d2d::LibraryList& list)
{
	initChoice();
}

wxSizer* ChangeImagePanel::createExtend(wxWindow* extend)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(createChooseListItem(extend));
	return sizer;	
}

wxSizer* ChangeImagePanel::createChooseListItem(wxWindow* extend)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

 	sizer->Add(new wxStaticText(extend, wxID_ANY, wxT("Change Image to ")));

 	m_choice = new wxChoice(extend, wxID_ANY);
	Connect(m_choice->GetId(), wxEVT_COMMAND_CHOICE_SELECTED, 
		wxCommandEventHandler(ChangeImagePanel::onChoiceChanged));
	initChoice();
 	sizer->Add(m_choice, 0, wxALIGN_LEFT);

	return sizer;
}

void ChangeImagePanel::onChoiceChanged(wxCommandEvent& event)
{
	m_behavior->selected = m_list->getSymbol(event.GetSelection());
	Context::Instance()->stage->getDesignerPage()->updateCodePage();
}

void ChangeImagePanel::initChoice()
{
	std::vector<wxString> names;
	m_list->traverse(d2d::GetNameVisitor<d2d::ListItem>(names));

	m_choice->Clear();
	if (!names.empty()) 
		m_choice->SetSelection(0);
	for (size_t i = 0, n = names.size(); i < n; ++i)
	{
		m_choice->Append(names[i]);
		if (m_list->getSymbol(i) == m_behavior->selected)
			m_choice->SetSelection(i);
	}
}
