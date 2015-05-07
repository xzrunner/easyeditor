#include "LibraryPanel.h"
#include "LibraryPage.h"
#include "StagePanel.h"

#include "dataset/Layer.h"

#include <easyshape.h>

namespace lr
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: d2d::LibraryPanel(parent)
	, m_viewlist(NULL)
	, m_paste_op(NULL)
	, m_draw_line_op(NULL)
{
}

LibraryPanel::~LibraryPanel()
{
	m_paste_op->Release();
	m_draw_line_op->Release();
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
		layer->TraverseSprite(d2d::FetchAllVisitor<d2d::ISprite>(sprites), true);
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

void LibraryPanel::InitPages(StagePanel* stage, d2d::PropertySettingPanel* property) 
{
	m_paste_op = new d2d::PasteSymbolOP(stage, stage, this);

	d2d::OneFloatValue* capture_val = new d2d::OneFloatValueStatic(10);

	m_draw_line_op = new libshape::EditPolylineOP<libshape::DrawPenLineOP, d2d::SelectShapesOP>(stage, stage, property, capture_val, NULL);
	m_draw_poly_op = new libshape::EditPolylineOP<libshape::DrawComplexPolygonOP, d2d::SelectShapesOP>(stage, stage, property, capture_val, NULL);

	addPage(new LibraryPage(m_notebook, "地形", m_paste_op));
	addPage(new LibraryPage(m_notebook, "装饰", m_paste_op));
	addPage(new LibraryPage(m_notebook, "单位", m_paste_op));
	addPage(new LibraryPage(m_notebook, "点", m_paste_op));
	addPage(new LibraryPage(m_notebook, "路径", m_draw_line_op));
	addPage(new LibraryPage(m_notebook, "区域", m_draw_poly_op));
	addPage(new LibraryPage(m_notebook, "碰撞区域", m_draw_poly_op));

	m_paste_op->Release();
	m_draw_line_op->Release();

	std::vector<Layer*> layers;
	for (int i = 0, n = m_pages.size(); i < n; ++i) {
		layers.push_back(static_cast<LibraryPage*>(m_pages[i])->GetLayer());
	}
	stage->SetLayers(layers);
}

void LibraryPanel::RefreshViewList()
{
	Layer* layer = static_cast<LibraryPage*>(m_selected)->GetLayer();
	std::vector<d2d::ISprite*> sprites;
	layer->TraverseSprite(d2d::FetchAllVisitor<d2d::ISprite>(sprites), true);
	m_viewlist->clear();
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		m_viewlist->insert(sprites[i]);
	}
}

}