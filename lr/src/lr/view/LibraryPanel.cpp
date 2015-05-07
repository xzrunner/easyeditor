#include "LibraryPanel.h"
#include "LibraryPage.h"

#include "dataset/Layer.h"

namespace lr
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: d2d::LibraryPanel(parent)
	, m_viewlist(NULL)
{
	addPage(new LibraryPage(m_notebook, "地形"));
	addPage(new LibraryPage(m_notebook, "装饰"));
	addPage(new LibraryPage(m_notebook, "单位"));
	addPage(new LibraryPage(m_notebook, "点"));
	addPage(new LibraryPage(m_notebook, "路径"));
	addPage(new LibraryPage(m_notebook, "区域"));
	addPage(new LibraryPage(m_notebook, "碰撞区域"));
}

void LibraryPanel::onPageChanged(wxBookCtrlEvent& event)
{
	d2d::LibraryPanel::onPageChanged(event);
	RefreshViewList();
}

void LibraryPanel::InitFromLayers(const std::vector<Layer*>& layers)
{
	for (int i = 0, n = m_pages.size(); i < n; ++i)
	{
		d2d::ILibraryPage* page = m_pages[i];
		Layer* layer = NULL;
		for (int i = 0, n = layers.size(); i < n; ++i) {
			if (page->getName() == layers[i]->GetName()) {
				layer = layers[i];
			}
		}
		assert(layer);

		static_cast<LibraryPage*>(page)->SetLayer(layer);

		std::vector<d2d::ISprite*> sprites;
		layer->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites), true);
		std::set<d2d::ISymbol*> symbol_set;
		for (int i = 0, n = sprites.size(); i < n; ++i) {
			d2d::ISymbol* symbol = const_cast<d2d::ISymbol*>(&sprites[i]->getSymbol());
			symbol->RefreshThumbnail(symbol->getFilepath());
			symbol_set.insert(symbol);
		}
		std::set<d2d::ISymbol*>::iterator itr = symbol_set.begin();
		for ( ; itr != symbol_set.end(); ++itr) {
			page->getList()->insert(*itr);
		}
	}
}

void LibraryPanel::RefreshViewList()
{
	Layer* layer = static_cast<LibraryPage*>(m_selected)->GetLayer();
	std::vector<d2d::ISprite*> sprites;
	layer->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites), true);
	m_viewlist->clear();
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		m_viewlist->insert(sprites[i]);
	}
}

}