#include "LibraryPanel.h"
#include "LibraryPage.h"
#include "StagePanel.h"

#include "dataset/Layer.h"

#include <easyshape.h>

namespace lr
{

BEGIN_EVENT_TABLE(LibraryPanel, wxPanel)
 	EVT_CHAR_HOOK(LibraryPanel::CharHook)
END_EVENT_TABLE()

LibraryPanel::LibraryPanel(wxWindow* parent)
	: d2d::LibraryPanel(parent)
	, m_viewlist(NULL) 
	, m_grouptree(NULL)
	, m_stage(NULL)
{
}

void LibraryPanel::OnPageChanged(wxBookCtrlEvent& event)
{
	d2d::LibraryPanel::OnPageChanged(event);

	Layer* curr = m_stage->GetCurrLayer();
	curr->SetEditable(true);
	static_cast<LibraryPage*>(m_pages[event.GetSelection()])->UpdateStatusFromLayer();

	Refresh();
}

void LibraryPanel::OnPageChanging(wxBookCtrlEvent& event)
{
	d2d::LibraryPanel::OnPageChanging(event);

	Layer* curr = m_stage->GetCurrLayer();
	curr->SetEditable(false);
	static_cast<LibraryPage*>(m_pages[event.GetSelection()])->UpdateStatusFromLayer();
}

void LibraryPanel::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	int layer_idx = 0;
	Json::Value layer_val = value[layer_idx++];
	while (!layer_val.isNull()) {
		d2d::LibraryList* list = m_pages[layer_idx-1]->GetList();
		int item_idx = 0;
		Json::Value item_val = layer_val[item_idx++];
		while (!item_val.isNull()) {
			std::string item_path = item_val.asString();
			std::string filepath = d2d::FilenameTools::getAbsolutePath(dir, item_path);
			try {
				d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
				symbol->RefreshThumbnail(symbol->GetFilepath());
				list->Insert(symbol);
				symbol->Release();
			} catch (d2d::Exception& e) {
				throw d2d::Exception("Create symbol %s fail!", item_path.c_str());
			}
			item_val = layer_val[item_idx++];
		}

		layer_val = value[layer_idx++];
	}
}

void LibraryPanel::StoreToFile(Json::Value& value, const std::string& dir) const
{
	for (int i = 0, n = m_pages.size(); i < n; ++i)
	{
		d2d::LibraryList* list = m_pages[i]->GetList();
		int j = 0;
		d2d::ISymbol* symbol = static_cast<d2d::ISymbol*>(list->GetItem(j++));
		while (symbol) {
			value[i][j-1] = d2d::FilenameTools::getRelativePath(dir, symbol->GetFilepath()).ToStdString();
			symbol = static_cast<d2d::ISymbol*>(list->GetItem(j++));
		}
	}
}

void LibraryPanel::InitFromLayers(const std::vector<Layer*>& layers)
{
	if (layers.size() != m_pages.size()) {
		throw d2d::Exception("lr json file error!");
	}

	for (int i = 0, n = m_pages.size(); i < n; ++i)
	{
		d2d::ILibraryPage* page = m_pages[i];

		Layer* layer = layers[i];

		static_cast<LibraryPage*>(page)->SetLayer(layer);
		layer->SetName(page->GetPageName().ToStdString());

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
 			d2d::ISymbol* symbol = const_cast<d2d::ISymbol*>(&sprites[i]->GetSymbol());
 			symbol_set.insert(symbol);
 		}
 
 		std::set<d2d::ISymbol*>::iterator itr = symbol_set.begin();
 		for ( ; itr != symbol_set.end(); ++itr) {
 			d2d::ISymbol* symbol = *itr;
 			symbol->RefreshThumbnail(symbol->GetFilepath());
 			page->GetList()->Insert(symbol);
 		}
	}
}

void LibraryPanel::InitPages(StagePanel* stage, d2d::PropertySettingPanel* property,
							 d2d::ViewPanelMgr* view_panel_mgr) 
{
	d2d::AbstractEditOP* paste_op = new d2d::PasteSymbolOP(stage, stage, this);

	d2d::OneFloatValue* capture_val = new d2d::OneFloatValueStatic(10);
	d2d::AbstractEditOP* draw_line_op = new libshape::EditPolylineOP<libshape::DrawPenLineOP, d2d::SelectShapesOP>(stage, stage, property, view_panel_mgr, capture_val, NULL);
	d2d::AbstractEditOP* draw_poly_op = new libshape::EditPolylineOP<libshape::DrawPolygonOP, d2d::SelectShapesOP>(stage, stage, property, view_panel_mgr, capture_val, NULL);

	{
		LibraryPage* page = new LibraryPage(m_notebook, "地形");
		Layer* layer = page->GetLayer();
		page->AddEditOP(m_stage->GetBaseOP());
		page->AddEditOP(paste_op);
		AddPage(page);
	}
	{
		LibraryPage* page = new LibraryPage(m_notebook, "装饰");
		Layer* layer = page->GetLayer();
		page->AddEditOP(m_stage->GetBaseOP());
		page->AddEditOP(paste_op);
		AddPage(page);
	}
	{
		LibraryPage* page = new LibraryPage(m_notebook, "单位");
		Layer* layer = page->GetLayer();
		page->AddEditOP(m_stage->GetBaseOP());
		page->AddEditOP(paste_op);
		AddPage(page);
	}
	{
		LibraryPage* page = new LibraryPage(m_notebook, "点");
		Layer* layer = page->GetLayer();
		page->AddEditOP(m_stage->GetBaseOP());
		page->AddEditOP(paste_op);
		AddPage(page);
	}
	{
		LibraryPage* page = new LibraryPage(m_notebook, "路径");
		Layer* layer = page->GetLayer();
		page->AddEditOP(m_stage->GetBaseOP());
		page->AddEditOP(draw_line_op);
		AddPage(page);
	}
	{
		LibraryPage* page = new LibraryPage(m_notebook, "区域");
		Layer* layer = page->GetLayer();
		page->AddEditOP(m_stage->GetBaseOP());
		page->AddEditOP(draw_poly_op);
		AddPage(page);
	}
	{
		LibraryPage* page = new LibraryPage(m_notebook, "碰撞区域");
		Layer* layer = page->GetLayer();
		page->AddEditOP(m_stage->GetBaseOP());
		page->AddEditOP(draw_poly_op);
		page->AddEditOP(draw_line_op);
		AddPage(page);
	}
	{
		LibraryPage* page = new LibraryPage(m_notebook, "摄像机");
		Layer* layer = page->GetLayer();
		page->AddEditOP(m_stage->GetBaseOP());
		page->AddEditOP(paste_op);
		AddPage(page);
	}

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

void LibraryPanel::Refresh()
{
	Layer* layer = static_cast<LibraryPage*>(m_selected)->GetLayer();

	std::vector<d2d::ISprite*> sprites;
	layer->TraverseSprite(d2d::FetchAllVisitor<d2d::ISprite>(sprites), true);

	// stage
	m_stage->GetSpriteSelection()->Clear();
	m_stage->GetShapeSelection()->Clear();

	// view list
	m_viewlist->Clear();
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		m_viewlist->Insert(sprites[i]);
	}

	// group tree
	m_grouptree->Clear();
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		m_grouptree->InsertSprite(sprites[i]);
	}
}

void LibraryPanel::CharHook(wxKeyEvent& event)
{
	int key_code = event.GetKeyCode();
	if (key_code >= '1' && key_code <= '8') {
		int idx = key_code - '1';
		SetCurrPage(idx);
		return;
	} else {
		event.Skip();
	}
}

}