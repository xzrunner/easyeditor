#include "LibraryPanel.h"
#include "LibraryPage.h"
#include "StagePanel.h"

#include "dataset/Layer.h"

#include <easyshape.h>

namespace lr
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: d2d::LibraryPanel(parent)
{
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

		Layer* layer = layers[i];

//		Layer* layer = NULL;
// 		for (int i = 0, n = layers.size(); i < n; ++i) {
// 			if (page->getName() == layers[i]->GetName()) {
// 				layer = layers[i];
// 			}
// 		}
// 		if (!layer) {
// 			throw d2d::Exception("layer error: %s", page->getName());
// 		}

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
	d2d::AbstractEditOP* paste_op = new d2d::PasteSymbolOP(stage, stage, this);

	d2d::OneFloatValue* capture_val = new d2d::OneFloatValueStatic(10);
	d2d::AbstractEditOP* draw_line_op = new libshape::EditPolylineOP<libshape::DrawPenLineOP, d2d::SelectShapesOP>(stage, stage, property, capture_val, NULL);
	d2d::AbstractEditOP* draw_poly_op = new libshape::EditPolylineOP<libshape::DrawPolygonOP, d2d::SelectShapesOP>(stage, stage, property, capture_val, NULL);

	addPage(new LibraryPage(m_notebook, "地形", paste_op));
	addPage(new LibraryPage(m_notebook, "装饰", paste_op));
	addPage(new LibraryPage(m_notebook, "单位", paste_op));
	addPage(new LibraryPage(m_notebook, "点", paste_op));
	addPage(new LibraryPage(m_notebook, "路径", draw_line_op));
	addPage(new LibraryPage(m_notebook, "区域", draw_poly_op));
	addPage(new LibraryPage(m_notebook, "碰撞区域", draw_poly_op));
	addPage(new LibraryPage(m_notebook, "摄像机", paste_op));

	paste_op->Release();
	draw_line_op->Release();
	draw_poly_op->Release();

	std::vector<Layer*> layers;
	for (int i = 0, n = m_pages.size(); i < n; ++i) {
		Layer* layer = static_cast<LibraryPage*>(m_pages[i])->GetLayer();
		layer->Retain();
		layers.push_back(layer);
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