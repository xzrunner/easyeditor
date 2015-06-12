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
	, m_stage(NULL)
{
}

void LibraryPanel::onPageChanged(wxBookCtrlEvent& event)
{
	d2d::LibraryPanel::onPageChanged(event);
	RefreshViewList();
}

void LibraryPanel::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	int layer_idx = 0;
	Json::Value layer_val = value[layer_idx++];
	while (!layer_val.isNull()) {
		d2d::LibraryList* list = m_pages[layer_idx-1]->getList();
		int item_idx = 0;
		Json::Value item_val = layer_val[item_idx++];
		while (!item_val.isNull()) {
			std::string filepath = d2d::FilenameTools::getAbsolutePath(dir, item_val.asString());
			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
			symbol->RefreshThumbnail(symbol->getFilepath());
			list->insert(symbol);
			symbol->Release();

			item_val = layer_val[item_idx++];
		}

		layer_val = value[layer_idx++];
	}
}

void LibraryPanel::StoreToFile(Json::Value& value, const std::string& dir) const
{
	for (int i = 0, n = m_pages.size(); i < n; ++i)
	{
		d2d::LibraryList* list = m_pages[i]->getList();
		int j = 0;
		d2d::ISymbol* symbol = list->getSymbol(j++);
		while (symbol) {
			value[i][j-1] = d2d::FilenameTools::getRelativePath(dir, symbol->getFilepath()).ToStdString();
			symbol = list->getSymbol(j++);
		}
	}
}

void LibraryPanel::InitFromLayers(const std::vector<Layer*>& layers)
{
	for (int i = 0, n = m_pages.size(); i < n; ++i)
	{
		d2d::ILibraryPage* page = m_pages[i];

		Layer* layer = layers[i];

		static_cast<LibraryPage*>(page)->SetLayer(layer);
		layer->SetName(page->getName().ToStdString());

		static_cast<LibraryPage*>(page)->UpdateStatusFromLayer();
	}
}

void LibraryPanel::LoadSymbolFromLayer()
{
	for (int i = 0, n = m_pages.size(); i < n; ++i)
	{
		LibraryPage* page = static_cast<LibraryPage*>(m_pages[i]);

 		std::vector<d2d::ISprite*> sprites;
 		page->GetLayer()->TraverseSprite(d2d::FetchAllVisitor<d2d::ISprite>(sprites), true);
 		std::set<d2d::ISymbol*> symbol_set;
 		for (int i = 0, n = sprites.size(); i < n; ++i) {
 			d2d::ISymbol* symbol = const_cast<d2d::ISymbol*>(&sprites[i]->getSymbol());
 			symbol_set.insert(symbol);
 		}
 
 		std::set<d2d::ISymbol*>::iterator itr = symbol_set.begin();
 		for ( ; itr != symbol_set.end(); ++itr) {
 			d2d::ISymbol* symbol = *itr;
 			symbol->RefreshThumbnail(symbol->getFilepath());
 			page->getList()->insert(symbol);
 		}
	}
}

void LibraryPanel::InitPages(StagePanel* stage, d2d::PropertySettingPanel* property) 
{
	d2d::AbstractEditOP* paste_op = new d2d::PasteSymbolOP(stage, stage, this);

	d2d::OneFloatValue* capture_val = new d2d::OneFloatValueStatic(10);
	d2d::AbstractEditOP* draw_line_op = new libshape::EditPolylineOP<libshape::DrawPenLineOP, d2d::SelectShapesOP>(stage, stage, property, capture_val, NULL);
	d2d::AbstractEditOP* draw_poly_op = new libshape::EditPolylineOP<libshape::DrawPolygonOP, d2d::SelectShapesOP>(stage, stage, property, capture_val, NULL);

	AddPage(new LibraryPage(m_notebook, "地形", paste_op));
	AddPage(new LibraryPage(m_notebook, "装饰", paste_op));
	AddPage(new LibraryPage(m_notebook, "单位", paste_op));
	AddPage(new LibraryPage(m_notebook, "点", paste_op));
	AddPage(new LibraryPage(m_notebook, "路径", draw_line_op));
	AddPage(new LibraryPage(m_notebook, "区域", draw_poly_op));
	AddPage(new LibraryPage(m_notebook, "碰撞区域", draw_poly_op));
	AddPage(new LibraryPage(m_notebook, "摄像机", paste_op));

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

	m_viewlist->Clear();
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		m_viewlist->Insert(sprites[i]);
	}

	m_stage->getSpriteSelection()->Clear();
	m_stage->getShapeSelection()->Clear();
}

}