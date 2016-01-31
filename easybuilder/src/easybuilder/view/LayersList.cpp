
#include "LayersList.h"
#include "LayersPage.h"
#include "LayerItem.h"
#include "PropertySettingPanel.h"
#include "LayerPropertySetting.h"

#include "dataset/Scene.h"
#include "output/DesignerPage.h"
#include "view/Context.h"
#include "view/StagePanel.h"
#include "view/LibraryPanel.h"
#include "view/ActorsList.h"

using namespace ebuilder;

LayersList::LayersList(LayersPage* page)
	: ee::LibraryList(page, "layer", false)
	, m_page(page)
{
}

void LayersList::onListSelected(wxCommandEvent& event)
{
	onSelected(event.GetInt());
}

void LayersList::onListDoubleClicked(wxCommandEvent& event)
{
	onSelected(event.GetInt());
}

void LayersList::loadFromScene(Scene* scene)
{
	clear();

	const std::vector<Layer*>& layers = scene->getAllLayers();
	for (size_t i = 0, n = layers.size(); i < n; ++i)
	{
		LayerItem* layer = new LayerItem(layers[i]);
		layer->refresh();
		insert(layer);
	}
}

void LayersList::onSelected(int index)
{
	if (m_items.empty()) return;

	Context* context = Context::Instance();

	// property panel
	LayerItem* item = static_cast<LayerItem*>(m_items[index]);
	ee::IPropertySetting* setting 
		= new LayerPropertySetting(item, m_page);
	context->property->setPropertySetting(setting);

	// stage panel
	DesignerPage* stage = context->stage->getDesignerPage();
	stage->getEditOP()->clear();
	stage->Refresh();

	m_page->onSelect(index, GetItemCount());

	// library panel
	LibraryPanel* library = context->library;
	static_cast<ActorsList*>(library->getActorList())
		->loadFromLayer(item->getLayer());
}