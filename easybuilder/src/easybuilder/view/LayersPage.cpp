
#include "LayersPage.h"
#include "LayersList.h"
#include "LayerItem.h"
#include "SceneItem.h"
#include "Context.h"
#include "StagePanel.h"
#include "LibraryPanel.h"

#include "dataset/Layer.h"
#include "dataset/Scene.h"
#include "output/DesignerPage.h"

using namespace ebuilder;

LayersPage::LayersPage(wxWindow* parent)
	: ee::ILibraryPage(parent, wxT("Layers"))
{
	initLayout();
}

bool LayersPage::isHandleSymbol(ee::ISymbol* symbol) const
{
	return dynamic_cast<LayerItem*>(symbol) != NULL;
}

void LayersPage::refreshThumbnail()
{
	if (m_list)
		m_list->getSymbol()->refresh();
}

void LayersPage::insert()
{
	LayerItem* item = new LayerItem(new Layer);
	m_list->insert(item);

	SceneItem* scene = dynamic_cast<SceneItem*>(Context::Instance()->library->getSceneList()->getItem());
	assert(scene);
	scene->getScene()->insert(item->getLayer());
}

void LayersPage::enableDelBtn()
{
	m_btnDel->Enable(true);
}

void LayersPage::onSelect(int index, int size)
{
	m_btnUp->Enable(true);
	m_btnDown->Enable(true);

	if (index == 0)
		m_btnUp->Enable(false);
	if (index == size - 1)
		m_btnDown->Enable(false);
}

void LayersPage::initLayout(bool draggable /*= true*/)
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	{
		wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

		m_btnUp = new wxButton(this, wxID_ANY, "up", wxDefaultPosition, wxSize(40, 20));
		Connect(m_btnUp->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LayersPage::onUpPress));
		btnSizer->Add(m_btnUp, 0, wxRIGHT, 10);
		m_btnUp->Enable(false);

		m_btnDown = new wxButton(this, wxID_ANY, "down", wxDefaultPosition, wxSize(40, 20));
		Connect(m_btnDown->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LayersPage::onDownPress));
		btnSizer->Add(m_btnDown, 0, wxRIGHT, 10);
		m_btnDown->Enable(false);

		initButtons(btnSizer);
		m_btnDel->Enable(false);

		sizer->Add(btnSizer, 0, wxALIGN_RIGHT);
	}
	{
		if (!m_list)
			m_list = new LayersList(this);
		sizer->Add(m_list, 1, wxEXPAND);
	}
	SetSizer(sizer);
}

void LayersPage::onAddPress(wxCommandEvent& event)
{
	insert();

	m_btnDel->Enable(true);
}

void LayersPage::onDelPress(wxCommandEvent& event)
{
	LibraryPanel* library = Context::Instance()->library;
	LayerItem* layer = dynamic_cast<LayerItem*>(library->getLayerList()->getItem());
	SceneItem* scene = dynamic_cast<SceneItem*>(library->getSceneList()->getItem());
	scene->getScene()->remove(layer->getLayer());
	scene->refresh();
	
	m_list->remove();

	if (m_list->GetItemCount() <= 1)
	{
		m_btnDel->Enable(false);
		m_btnUp->Enable(false);
		m_btnDown->Enable(false);
	}
}

void LayersPage::onUpPress(wxCommandEvent& event)
{
	int index = m_list->GetSelection();
	m_list->swap(index, index - 1);

	DesignerPage* designer = Context::Instance()->stage->getDesignerPage();
	SceneItem* scene = dynamic_cast<SceneItem*>(Context::Instance()->library->getSceneList()->getItem());
	scene->getScene()->swapLayers(index, index - 1);
	scene->refresh();
	designer->Refresh();
}

void LayersPage::onDownPress(wxCommandEvent& event)
{
	int index = m_list->GetSelection();
	m_list->swap(index, index + 1);

	DesignerPage* designer = Context::Instance()->stage->getDesignerPage();
	SceneItem* scene = dynamic_cast<SceneItem*>(Context::Instance()->library->getSceneList()->getItem());
	scene->getScene()->swapLayers(index, index + 1);
	scene->refresh();
	designer->Refresh();
}