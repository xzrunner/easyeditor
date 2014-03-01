
#include "ActorsPage.h"
#include "ActorItem.h"
#include "ActorsList.h"

#include "dataset/Actor.h"
#include "dataset/Layer.h"
#include "view/Context.h"
#include "view/StagePanel.h"
#include "output/DesignerPage.h"

using namespace ebuilder;

ActorsPage::ActorsPage(wxWindow* parent)
	: d2d::ILibraryPage(parent, wxT("Actors"))
{
	initLayout();
}

bool ActorsPage::isHandleSymbol(d2d::ISymbol* symbol) const
{
	return dynamic_cast<ActorItem*>(symbol) != NULL;
}

void ActorsPage::initLayout(bool draggable /*= true*/)
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	initButtons(sizer);

	if (!m_list)
		m_list = new ActorsList(this);
	sizer->Add(m_list, 1, wxEXPAND);

	SetSizer(sizer);
}

void ActorsPage::onAddPress(wxCommandEvent& event)
{
}

void ActorsPage::onDelPress(wxCommandEvent& event)
{
	Context::Instance()->stage->getDesignerPage()->getSpriteSelection()->clear();

	ActorItem* item = static_cast<ActorItem*>(m_list->getItem(m_list->GetSelection()));
	DesignerPage* designer = Context::Instance()->stage->getDesignerPage();
	designer->removeSprite(item->getActor());
	designer->Refresh();

	d2d::ILibraryPage::onDelPress(event);
}

void ActorsPage::initButtons(wxSizer* sizer)
{
	wxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

// 	m_btnAdd = new wxButton(this, wxID_ANY, "+", wxDefaultPosition, wxSize(20, 20));
// 	Connect(m_btnAdd->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ILibraryPage::onAddPress));
// 	btnSizer->Add(m_btnAdd, 0, wxLEFT | wxRIGHT, 5);

	m_btnDel = new wxButton(this, wxID_ANY, "-", wxDefaultPosition, wxSize(20, 20));
	Connect(m_btnDel->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
		wxCommandEventHandler(ActorsPage::onDelPress));
	btnSizer->Add(m_btnDel, 0, wxLEFT | wxRIGHT, 5);

	sizer->Add(btnSizer, 0, wxALIGN_RIGHT);
}
