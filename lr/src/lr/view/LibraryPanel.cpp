#include "LibraryPanel.h"
#include "LibraryPage.h"
#include "StagePanel.h"

#include "dataset/Layer.h"

#include <ee/panel_msg.h>
#include <ee/FileHelper.h>
#include <ee/LibraryList.h>
#include <ee/SymbolMgr.h>
#include <ee/Exception.h>
#include <ee/FetchAllVisitor.h>
#include <ee/PasteSymbolOP.h>
#include <ee/SelectShapesOP.h>
#include <ee/SpriteSelection.h>
#include <ee/ShapeSelection.h>
#include <ee/ViewlistPanel.h>
#include <ee/GroupTreePanel.h>

#include <easyshape.h>

namespace lr
{

BEGIN_EVENT_TABLE(LibraryPanel, wxPanel)
 	EVT_CHAR_HOOK(LibraryPanel::CharHook)
END_EVENT_TABLE()

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee::LibraryPanel(parent)
	, m_viewlist(NULL) 
	, m_grouptree(NULL)
	, m_stage(NULL)
{
	m_terrain_page = m_unit_page = m_path_page = m_level_page = NULL;
}

void LibraryPanel::OnPageChanged(wxBookCtrlEvent& event)
{
	ee::LibraryPanel::OnPageChanged(event);

	Layer* curr = static_cast<LibraryPage*>(GetCurrPage())->GetLayer();
	ee::ChangeLayerMgrSJ::Instance()->Change(curr->GetLayerMgr());
	curr->SetEditable(true);
	static_cast<LibraryPage*>(m_pages[event.GetSelection()])->UpdateStatusFromLayer();

	Refresh();
}

void LibraryPanel::OnPageChanging(wxBookCtrlEvent& event)
{
	ee::LibraryPanel::OnPageChanging(event);

	Layer* curr = static_cast<LibraryPage*>(GetCurrPage())->GetLayer();
	curr->SetEditable(false);
	static_cast<LibraryPage*>(m_pages[event.GetSelection()])->UpdateStatusFromLayer();
}

void LibraryPanel::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	for (int i = 0, n = value.size(); i < n; ++i)
	{
		Json::Value layer_val = value[i];
		if (layer_val.isNull()) {
			continue;
		}

		ee::LibraryList* list = m_pages[i]->GetList();
		int item_idx = 0;
		Json::Value item_val = layer_val[item_idx++];
		while (!item_val.isNull()) {
			std::string item_path = item_val.asString();
			std::string filepath = ee::FileHelper::GetAbsolutePath(dir, item_path);
			try {
				ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
				if (!sym) {
					item_val = layer_val[item_idx++];
					continue;
				}
				sym->RefreshThumbnail(sym->GetFilepath());
				list->Insert(sym);
				sym->RemoveReference();
			} catch (ee::Exception& e) {
				item_val = layer_val[item_idx++];
				continue;
			}
			item_val = layer_val[item_idx++];
		}
	}
}

void LibraryPanel::StoreToFile(Json::Value& value, const std::string& dir) const
{
	for (int i = 0, n = m_pages.size(); i < n; ++i)
	{
		ee::LibraryList* list = m_pages[i]->GetList();
		int j = 0;
		ee::Symbol* sym = static_cast<ee::Symbol*>(list->GetItem(j++));
		while (sym) {
			value[i][j-1] = ee::FileHelper::GetRelativePath(dir, sym->GetFilepath());
			sym = static_cast<ee::Symbol*>(list->GetItem(j++));
		}
	}
}

void LibraryPanel::InitFromLayers(const std::vector<Layer*>& layers)
{
	assert(layers.size() <= m_pages.size());
	for (int i = 0, n = layers.size(); i < n; ++i)
	{
		Layer* layer = layers[i];
		ee::LibraryPage* page = m_pages[i];

		static_cast<LibraryPage*>(page)->SetLayer(layer);
		layer->SetName(page->GetPageName());

		static_cast<LibraryPage*>(page)->UpdateStatusFromLayer();
	}
}

void LibraryPanel::LoadSymbolFromLayer()
{
	for (int i = 0, n = m_pages.size(); i < n; ++i)
	{
		LibraryPage* page = static_cast<LibraryPage*>(m_pages[i]);

 		std::vector<ee::Sprite*> sprs;
 		page->GetLayer()->TraverseSprite(ee::FetchAllVisitor<ee::Sprite>(sprs), true);
 		std::set<ee::Symbol*> symbol_set;
 		for (int i = 0, n = sprs.size(); i < n; ++i) {
 			ee::Symbol* sym = dynamic_cast<ee::Symbol*>(sprs[i]->GetSymbol());
 			symbol_set.insert(sym);
 		}
 
 		std::set<ee::Symbol*>::iterator itr = symbol_set.begin();
 		for ( ; itr != symbol_set.end(); ++itr) {
 			ee::Symbol* sym = *itr;
 			sym->RefreshThumbnail(sym->GetFilepath());
 			page->GetList()->Insert(sym);
 		}
	}
}

void LibraryPanel::InitPages(StagePanel* stage, ee::PropertySettingPanel* property) 
{
	ee::EditOP* paste_op = new ee::PasteSymbolOP(stage, stage->GetStageImpl(), this);

	ee::OneFloatValue* capture_val = new ee::OneFloatValueStatic(10);
	ee::EditOP* draw_line_op = new eshape::EditPolylineOP<eshape::DrawPenLineOP, ee::SelectShapesOP>(stage, stage->GetStageImpl(), stage, property, capture_val, NULL);
	ee::EditOP* draw_poly_op = new eshape::EditPolylineOP<eshape::DrawPolygonOP, ee::SelectShapesOP>(stage, stage->GetStageImpl(), stage, property, capture_val, NULL);

	int id = 0;
	{
		LibraryPage* page = new LibraryPage(this, "地形", LT_DEFAULT, id++, s2::CM_PERSPECTIVE_NO_HEIGHT);
		Layer* layer = page->GetLayer();
		page->AddEditOP(m_stage->GetBaseOP());
		page->AddEditOP(paste_op);
		AddPage(page);
		m_terrain_page = page;
	}
	{
		LibraryPage* page = new LibraryPage(this, "装饰", LT_DEFAULT, id++, s2::CM_PERSPECTIVE_AUTO_HEIGHT);
		Layer* layer = page->GetLayer();
		page->AddEditOP(m_stage->GetBaseOP());
		page->AddEditOP(paste_op);
		AddPage(page);
	}
	{
		LibraryPage* page = new LibraryPage(this, "单位", LT_DEFAULT, id++, s2::CM_PERSPECTIVE_AUTO_HEIGHT);
		Layer* layer = page->GetLayer();
		page->AddEditOP(m_stage->GetBaseOP());
		page->AddEditOP(paste_op);
		AddPage(page);
		m_unit_page = page;
	}
	{
		LibraryPage* page = new LibraryPage(this, "点", LT_DEFAULT, id++, s2::CM_PERSPECTIVE_NO_HEIGHT);
		Layer* layer = page->GetLayer();
		page->AddEditOP(m_stage->GetBaseOP());
		page->AddEditOP(paste_op);
		AddPage(page);
	}
	{
		LibraryPage* page = new LibraryPage(this, "路径", LT_DEFAULT, id++, s2::CM_PERSPECTIVE_NO_HEIGHT);
		Layer* layer = page->GetLayer();
		page->AddEditOP(m_stage->GetBaseOP());
		page->AddEditOP(draw_line_op);
		AddPage(page);
		m_path_page = page;
	}
	{
		LibraryPage* page = new LibraryPage(this, "区域", LT_SHAPE, id++, s2::CM_PERSPECTIVE_NO_HEIGHT);
		Layer* layer = page->GetLayer();
		page->AddEditOP(m_stage->GetBaseOP());
		page->AddEditOP(draw_poly_op);
		AddPage(page);
	}
	{
		LibraryPage* page = new LibraryPage(this, "碰撞区域", LT_SHAPE, id++, s2::CM_PERSPECTIVE_NO_HEIGHT);
		Layer* layer = page->GetLayer();
		page->AddEditOP(m_stage->GetBaseOP());
		page->AddEditOP(draw_poly_op);
		page->AddEditOP(draw_line_op);
		AddPage(page);
		m_collision_page = page;
	}
	{
		LibraryPage* page = new LibraryPage(this, "摄像机", LT_DEFAULT, id++, s2::CM_PERSPECTIVE_NO_HEIGHT);
		Layer* layer = page->GetLayer();
		page->AddEditOP(m_stage->GetBaseOP());
		page->AddEditOP(paste_op);
		AddPage(page);
	}
	{
		LibraryPage* page = new LibraryPage(this, "关卡", LT_DEFAULT, id++, s2::CM_PERSPECTIVE_NO_HEIGHT);
		Layer* layer = page->GetLayer();
		page->AddEditOP(m_stage->GetBaseOP());
		page->AddEditOP(paste_op);
		AddPage(page);
		m_level_page = page;
	}

	paste_op->RemoveReference();
	draw_line_op->RemoveReference();
	draw_poly_op->RemoveReference();

	std::vector<Layer*> layers;
	for (int i = 0, n = m_pages.size(); i < n; ++i) {
		Layer* layer = static_cast<LibraryPage*>(m_pages[i])->GetLayer();
		layer->AddReference();
		layers.push_back(layer);
	}
	stage->SetLayers(layers);
}

void LibraryPanel::Refresh()
{
	Layer* layer = static_cast<LibraryPage*>(m_selected)->GetLayer();

	std::vector<ee::Sprite*> sprs;
	layer->TraverseSprite(ee::FetchAllVisitor<ee::Sprite>(sprs), true);

	// stage
	m_stage->GetSpriteSelection()->Clear();
	m_stage->GetShapeSelection()->Clear();

	// view list
	m_viewlist->Clear();
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		m_viewlist->Insert(sprs[i]);
	}

	// group tree
	m_grouptree->EnableExpand(false);
	m_grouptree->Clear();
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		m_grouptree->Insert(sprs[i]);
	}
	m_grouptree->EnableExpand(true);
}

Layer* LibraryPanel::GetTerrainLayer()
{
	return m_terrain_page->GetLayer();
}

Layer* LibraryPanel::GetCollisionLayer()
{
	return m_collision_page->GetLayer();
}

LayerType LibraryPanel::GetLayerType(int idx) const
{
	if (idx < 0 || idx >= m_pages.size()) {
		return LT_DEFAULT;
	} else {
		return static_cast<LibraryPage*>(m_pages[idx])->GetLayerType();
	}
}

s2::CameraMode LibraryPanel::GetLayerCameraMode(int idx) const
{
	if (idx < 0 || idx >= m_pages.size()) {
		return s2::CM_ORTHO;
	} else {
		return static_cast<LibraryPage*>(m_pages[idx])->GetLayerCameraMode();
	}	
}

Layer* LibraryPanel::GetLayer(int idx)
{
	if (idx < 0 || idx >= m_pages.size()) {
		return NULL;
	} else {
		return static_cast<LibraryPage*>(m_pages[idx])->GetLayer();
	}
}

bool LibraryPanel::IsCurrUnitLayer()
{
	return m_unit_page == static_cast<LibraryPage*>(GetCurrPage());
}

bool LibraryPanel::IsCurrLevelLayer()
{
	return m_level_page == static_cast<LibraryPage*>(GetCurrPage());
}

void LibraryPanel::GetAllPathName(std::vector<std::string>& names) const
{
	Layer* layer = static_cast<LibraryPage*>(m_path_page)->GetLayer();
	std::vector<ee::Shape*> shapes;
	layer->TraverseShape(ee::FetchAllVisitor<ee::Shape>(shapes));
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		names.push_back(shapes[i]->GetName());
	}
}

void LibraryPanel::CharHook(wxKeyEvent& event)
{
	int key_code = event.GetKeyCode();
	if (key_code >= '1' && key_code <= '9') {
		int idx = key_code - '1';
		SetCurrPage(idx);
		return;
	} else {
		event.Skip();
	}
}

}