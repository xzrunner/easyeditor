
#include "ScenesPage.h"
#include "Context.h"
#include "ScenesList.h"
#include "SceneItem.h"
#include "ListObserver.h"
#include "StagePanel.h"

#include "dataset/Scene.h"
#include "output/DesignerPage.h"

using namespace ebuilder;

ScenesPage::ScenesPage(wxWindow* parent)
	: ee::ILibraryPage(parent, wxT("Scenes"))
{
	initLayout();

	m_observer = new ListObserver(m_list);
}

ScenesPage::~ScenesPage()
{
	delete m_observer;
}

bool ScenesPage::isHandleSymbol(ee::ISymbol* symbol) const
{
	return dynamic_cast<SceneItem*>(symbol) != NULL;
}

void ScenesPage::refreshThumbnail()
{
	if (m_list)
		m_list->getItem()->refresh();	
}

void ScenesPage::insert()
{
	SceneItem* item = new SceneItem(new Scene);
	m_list->insert(item);

	Context* context = Context::Instance();
	context->stage->addCodePage(item);
}

void ScenesPage::enableDelBtn()
{
	m_btnDel->Enable(true);
}

void ScenesPage::initLayout(bool draggable /*= true*/)
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

 	initButtons(sizer);
 	m_btnDel->Enable(false);

	if (!m_list)
		m_list = new ScenesList(this);
	sizer->Add(m_list, 1, wxEXPAND);

	SetSizer(sizer);
}

void ScenesPage::onAddPress(wxCommandEvent& event)
{
	insert();

	m_btnDel->Enable(true);

	m_observer->updateListeners();
}

void ScenesPage::onDelPress(wxCommandEvent& event)
{
	Context::Instance()->stage->removeCodePage(
		static_cast<SceneItem*>(m_list->getSymbol()));

	m_list->remove();

	if (m_list->GetItemCount() <= 1)
		m_btnDel->Enable(false);

	m_observer->updateListeners();
}
