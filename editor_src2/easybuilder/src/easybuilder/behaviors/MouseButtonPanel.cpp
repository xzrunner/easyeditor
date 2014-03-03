
#include "MouseButtonPanel.h"
#include "MouseButton.h"
#include "BehaviorItem.h"
#include "BehaviorFactory.h"
#include "CollapsiblePane.h"

#include "view/Context.h"
#include "view/StagePanel.h"
#include "output/DesignerPage.h"
#include "view/LibraryPanel.h"

#include <wx/statline.h>

using namespace ebuilder;

MouseButtonPanel::MouseButtonPanel(wxWindow* parent,
								   MouseButton* behavior)
	: BehaviorPanel(parent, MOUSE_BUTTON_NAME)
	, m_behavior(behavior)
{
}

wxSizer* MouseButtonPanel::createExtend(wxWindow* extend)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(createMouseStateListItem(extend));
//	sizer->Add(new wxStaticLine(extend), 0, wxEXPAND);
	sizer->Add(createSatisfyPanel(extend), 0, wxALL, 5);
//	sizer->Add(new wxStaticLine(extend), 0, wxEXPAND);
//	sizer->Add(createNotSatisfyPanel(extend));

	for (size_t i = 0, n = m_behavior->satisfies.size(); i < n; ++i)
	{
		Behavior* behavior = m_behavior->satisfies[i];
		BehaviorPanel* behaviorPanel = 
			BehaviorFactory::createBehaviorPanel(m_extendSatisfy, behavior);
		behaviorPanel->createPanel(m_extendSatisfy);
	}
// 	for (size_t i = 0, n = m_behavior->notSatisfies.size(); i < n; ++i)
// 	{
// 		Behavior* behavior = m_behavior->notSatisfies[i];
// 		BehaviorPanel* behaviorPanel = 
// 			BehaviorFactory::createBehaviorPanel(m_extendNotSatisfy, behavior);
// 		behaviorPanel->createPanel(m_extendNotSatisfy);
// 	}

	return sizer;
}

wxSizer* MouseButtonPanel::createMouseStateListItem(wxWindow* extend)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	sizer->Add(new wxStaticText(extend, wxID_ANY, wxT("Mouse Event ")));

	wxString strings[2];
	strings[0] = "press";
	strings[1] = "lift";
	wxChoice* choice = new wxChoice(extend, wxID_ANY, wxDefaultPosition, 
		wxDefaultSize, 2, strings);
	Connect(choice->GetId(), wxEVT_COMMAND_CHOICE_SELECTED, 
		wxCommandEventHandler(MouseButtonPanel::onChoiceChanged));
	choice->SetSelection(m_behavior->bPress ? 0 : 1);
	sizer->Add(choice);

	return sizer;
}

wxSizer* MouseButtonPanel::createSatisfyPanel(wxWindow* extend)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	CollapsiblePane* panel = new CollapsiblePane(extend, wxT("Satisfy"));

	panel->SetBackgroundColour(*wxYELLOW);

	m_extendSatisfy = panel->GetPane();
	m_extendSatisfy->SetDropTarget(
		new DragTarget(m_extendSatisfy, m_behavior->getActor(), m_behavior, true));
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	wxTextCtrl* text = new wxTextCtrl(panel->GetPane(), wxID_ANY, 
		TIP_DRAG_BEHAVIOR, wxDefaultPosition, TIP_SIZE, wxTE_READONLY);
	topSizer->Add(text);
	m_extendSatisfy->SetSizer(topSizer);

	sizer->Add(panel);
	return sizer;
}

wxSizer* MouseButtonPanel::createNotSatisfyPanel(wxWindow* extend)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	CollapsiblePane* panel = new CollapsiblePane(extend, wxT("Not Satisfy"));

	panel->SetBackgroundColour(*wxGREEN);

	m_extendNotSatisfy = panel->GetPane();
	m_extendNotSatisfy->SetDropTarget(
		new DragTarget(m_extendNotSatisfy, m_behavior->getActor(), m_behavior, false));
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	wxTextCtrl* text = new wxTextCtrl(panel->GetPane(), wxID_ANY, TIP_DRAG_BEHAVIOR, wxDefaultPosition, TIP_SIZE, wxTE_READONLY);
	topSizer->Add(text);
	m_extendNotSatisfy->SetSizer(topSizer);

	sizer->Add(panel);
	return sizer;
}

void MouseButtonPanel::onChoiceChanged(wxCommandEvent& event)
{
	m_behavior->bPress = (event.GetSelection() == 0);
	Context::Instance()->stage->getDesignerPage()->updateCodePage();
}

//////////////////////////////////////////////////////////////////////////
// class MouseButtonPanel::DragTarget
//////////////////////////////////////////////////////////////////////////

MouseButtonPanel::DragTarget::
DragTarget(wxWindow* parent, Actor* actor,
		   MouseButton* behavior, bool bSatisfy)
	: m_parent(parent)
	, m_actor(actor)
	, m_behavior(behavior)
	, m_bSatisfy(bSatisfy)
{
}

bool MouseButtonPanel::DragTarget::
OnDropText(wxCoord x, wxCoord y, const wxString& data)
{
	wxString sType = data.substr(0, data.find(","));
	wxString sIndex = data.substr(data.find(",") + 1);

	long index;
	sIndex.ToLong(&index);

	BehaviorType type = static_cast<BehaviorItem*>
		(Context::Instance()->library->getBehaviorList()->getItem(index))->type();

	Behavior* behavior = BehaviorFactory::createBehavior(type, m_actor);
	if (m_bSatisfy)
		m_behavior->satisfies.push_back(behavior);
	else
		m_behavior->notSatisfies.push_back(behavior);

	BehaviorPanel* behaviorPanel = 
		BehaviorFactory::createBehaviorPanel(m_parent, behavior);
	behaviorPanel->createPanel(m_parent);

	Context::Instance()->stage->getDesignerPage()->updateCodePage();

	return true;
}
