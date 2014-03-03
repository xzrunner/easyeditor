
#include "BackstagePanel.h"

#include "dataset/Actor.h"
#include "behaviors/BehaviorFactory.h"
#include "behaviors/BehaviorPanel.h"
#include "behaviors/BehaviorItem.h"
#include "view/Context.h"
#include "view/LibraryPanel.h"
#include "view/StagePanel.h"
#include "output/DesignerPage.h"

#include <wx/splitter.h>

using namespace ebuilder;

BackstagePanel::BackstagePanel(wxWindow* parent)
	: wxPanel(parent)
	, m_actor(NULL)
{
	initLayout();

	m_behaviorsPanel->SetDropTarget(new DragTarget(m_behaviorsPanel));
}

void BackstagePanel::initFromActor(Actor* actor)
{
	m_actor = actor;

	m_behaviorsPanel->GetSizer()->DeleteWindows();
	Context::Instance()->library->clearUIListeners();

	if (actor)
	{
		const std::vector<Behavior*>& behaviors = actor->getBehaviors();
		for (size_t i = 0, n = behaviors.size(); i < n; ++i)
		{
			BehaviorPanel* behaviorPanel 
				= BehaviorFactory::createBehaviorPanel(m_behaviorsPanel, behaviors[i]);
			behaviorPanel->createPanel(m_behaviorsPanel);
		}
	}
}

void BackstagePanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	initTitle(sizer);
	initSplitterWindow(sizer);
	SetSizer(sizer);
}

void BackstagePanel::initTitle(wxSizer* sizer)
{
	wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT("  Backstage"));
	title->SetForegroundColour(*wxWHITE);
	title->SetBackgroundColour(*wxBLACK);
	sizer->Add(title, 0, wxEXPAND);
}

void BackstagePanel::initSplitterWindow(wxSizer* sizer)
{
	{
		m_btnClear = new wxButton(this, wxID_ANY, "clear");
	 	Connect(m_btnClear->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(BackstagePanel::onClearPress));
	 	sizer->Add(m_btnClear, 0, wxCENTRE | wxUP | wxDOWN, 5);
	}
// 	// debug
// 	{
// 		wxChoice* choice = new wxChoice(this, wxID_ANY);
// 		Connect(choice->GetId(), wxEVT_COMMAND_CHOICE_SELECTED, 
// 			wxCommandEventHandler(BackstagePanel::onChoiceChanged));
// 		
// 		choice->Append(wxT("xx"));
// 		choice->Append(wxT("zz"));
// 
// 		sizer->Add(choice, 0, wxALIGN_LEFT);
// 	}
	{
		m_behaviorsPanel = new wxScrolledWindow(this, wxID_ANY, 
			wxDefaultPosition, wxDefaultSize, wxVSCROLL);
		m_behaviorsPanel->SetBackgroundColour(wxColour("#dfdfdf"));
		m_behaviorsPanel->SetSizer(new wxBoxSizer(wxVERTICAL));
		sizer->Add(m_behaviorsPanel, 1, wxEXPAND);
	}
}

void BackstagePanel::onClearPress(wxCommandEvent& event)
{
	if (m_actor)
		m_actor->clearBehaviors();

	m_behaviorsPanel->GetSizer()->DeleteWindows();
	Context::Instance()->library->clearUIListeners();

	Context::Instance()->stage->getDesignerPage()->updateCodePage();
}

// void BackstagePanel::onChoiceChanged(wxCommandEvent& event)
// {
// 	int zz = 0;
// }

//////////////////////////////////////////////////////////////////////////
// class BackstagePanel::DragTarget
//////////////////////////////////////////////////////////////////////////

BackstagePanel::DragTarget::
DragTarget(wxWindow* parent)
	: m_parent(parent)
{
}

bool BackstagePanel::DragTarget::
OnDropText(wxCoord x, wxCoord y, const wxString& data)
{
	Context* context = Context::Instance();
	if (!context->backstage->m_actor) return true;

	wxString sType = data.substr(0, data.find(","));
	wxString sIndex = data.substr(data.find(",") + 1);

	long index;
	sIndex.ToLong(&index);

	BehaviorType type = static_cast<BehaviorItem*>
		(context->library->getBehaviorList()->getItem(index))->type();

	Behavior* behavior = BehaviorFactory::createBehavior(type, context->backstage->m_actor);
	context->backstage->m_actor->addBehavior(behavior);

	BehaviorPanel* behaviorPanel 
		= BehaviorFactory::createBehaviorPanel(context->backstage->m_behaviorsPanel, behavior);
	behaviorPanel->createPanel(m_parent);

	context->stage->getDesignerPage()->updateCodePage();

	return true;
}
