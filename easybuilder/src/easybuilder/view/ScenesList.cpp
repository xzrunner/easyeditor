
#include "ScenesList.h"
#include "ScenesPage.h"
#include "SceneItem.h"
#include "PropertySettingPanel.h"
#include "ScenePropertySetting.h"

#include "dataset/Scene.h"
#include "output/DesignerPage.h"
#include "view/Context.h"
#include "view/StagePanel.h"
#include "view/LibraryPanel.h"
#include "view/LayersList.h"
#include "view/ActorsList.h"

using namespace ebuilder;

ScenesList::ScenesList(ScenesPage* page)
	: ee::LibraryList(page, "scene", false)
	, m_page(page)
{
}

void ScenesList::onListSelected(wxCommandEvent& event)
{
	onSelected(event.GetInt());
}

void ScenesList::onListDoubleClicked(wxCommandEvent& event)
{
	onSelected(event.GetInt());
}

void ScenesList::onSelected(int index)
{
	if (m_items.empty()) return;

	Context* context = Context::Instance();

	// property panel
	SceneItem* item = static_cast<SceneItem*>(m_items[index]);
	ee::IPropertySetting* setting 
		= new ScenePropertySetting(item, m_page);
	context->property->setPropertySetting(setting);

	// stage panel
	DesignerPage* stage = context->stage->getDesignerPage();
	stage->getEditOP()->clear();
	stage->Refresh();

	// library panel
	LibraryPanel* library = context->library;
	static_cast<LayersList*>(library->getLayerList())
		->loadFromScene(item->getScene());
	
	const std::vector<Layer*>& layers = item->getScene()->getAllLayers();
	ee::LibraryList* list = library->getActorList();
	if (layers.empty())
		list->clear();
	else
		static_cast<ActorsList*>(list)->loadFromLayer(layers[0]);
}