
#include "LibraryPanel.h"
#include "ActorItem.h"
#include "ListObserver.h"
#include "ScenesPage.h"
#include "LayersPage.h"
#include "ActorsPage.h"
#include "MediaPage.h"
#include "BehaviorsPage.h"

using namespace ebuilder;

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee::LibraryPanel(parent)
	, m_scenePage(NULL)
	, m_actorPage(NULL)
	, m_mediaPage(NULL)
	, m_behaviorPage(NULL)
{
	initLayout();
}

// void LibraryPanel::addActor(Actor* actor)
// {
// 	m_actorPage->getList()->insert(new ActorItem(actor));
// }

void LibraryPanel::clearUIListeners()
{
	m_mediaPage->getListObserver()->clearUIListeners();
	m_scenePage->getListObserver()->clearUIListeners();
}

ee::LibraryList* LibraryPanel::getActorList() 
{ 
	return m_actorPage->getList(); 
}

ee::LibraryList* LibraryPanel::getLayerList()
{
	return m_layerPage->getList();
}

ee::LibraryList* LibraryPanel::getSceneList() 
{ 
	return m_scenePage->getList(); 
}

ee::LibraryList* LibraryPanel::getMediaList() 
{ 
	return m_mediaPage->getList(); 
}

ee::LibraryList* LibraryPanel::getBehaviorList() 
{ 
	return m_behaviorPage->getList(); 
}

void LibraryPanel::initLayout()
{
	wxSizer* s = GetSizer();

	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	initTitle(sizer);
	initNotebook(sizer);
	initTabPages(sizer);

	SetSizer(sizer);
}

void LibraryPanel::initTitle(wxSizer* sizer)
{
	wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT("  Library"));
	title->SetForegroundColour(*wxWHITE);
	title->SetBackgroundColour(*wxBLACK);
	sizer->Add(title, 0, wxEXPAND);
}

void LibraryPanel::initNotebook(wxSizer* sizer)
{
	m_notebook = new wxNotebook(this, wxID_ANY);
	Connect(m_notebook->GetId(), wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxBookCtrlEventHandler(LibraryPanel::onPageChanged));

	sizer->Add(m_notebook, 1, wxEXPAND);
}

void LibraryPanel::initTabPages(wxSizer* sizer)
{
	addPage(m_scenePage = new ScenesPage(m_notebook));
	addPage(m_layerPage = new LayersPage(m_notebook));
	addPage(m_actorPage = new ActorsPage(m_notebook));
	addPage(m_mediaPage = new MediaPage(m_notebook));
	addPage(m_behaviorPage = new BehaviorsPage(m_notebook));
}
