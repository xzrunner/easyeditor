#include "StagePanel.h"
#include "StageCanvas.h"
#include "OceanMesh.h"
#include "FileIO.h"
#include "ToolBarPanel.h"
#include "Sprite.h"
#include "Symbol.h"

#include <ee/shape_msg.h>
#include <ee/FetchAllVisitor.h>
#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/SpriteFactory.h>
#include <ee/sprite_msg.h>
#include <ee/LibraryPanel.h>
#include <ee/panel_msg.h>
#include <ee/ImageSymbol.h>
#include <ee/SymbolPath.h>

#include <easyshape.h>

namespace eterrain2d
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), library)
	, ee::ShapesPanelImpl()
{
	SetEditOP(new ee::ZoomViewOP(this, GetStageImpl(), true));
	SetCanvas(new StageCanvas(this));

 	SetDropTarget(new StageDropTarget(this, library));
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   wxGLContext* glctx, const ee::SprPtr& edited, 
					   const ee::MultiSpritesImpl* bg_sprites, 
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), library)
	, ee::ShapesPanelImpl()
{
	SetEditOP(new ee::ZoomViewOP(this, GetStageImpl(), true));
	SetCanvas(new StageCanvas(this, glctx, edited, bg_sprites));

	m_oceans = dynamic_cast<Symbol*>(edited->GetSymbol())->GetOceans();
	for (int i = 0, n = m_oceans.size(); i < n; ++i) {
 		OceanMesh* ocean = m_oceans[i];
		ee::InsertShapeSJ::Instance()->Insert(
			const_cast<eshape::PolygonShape*>(ocean->GetBounding()));
	}

	SetDropTarget(new StageDropTarget(this, library));
}

StagePanel::~StagePanel()
{	
	for (int i = 0, n = m_oceans.size(); i < n; ++i) {
		m_oceans[i]->RemoveReference();
	}
}

bool StagePanel::UpdateStage()
{
	return true;
}

void StagePanel::Store(const std::string& dir, Json::Value& value) const
{
	std::vector<ee::SprPtr> bg_sprites;
	TraverseSprites(ee::FetchAllRefVisitor<ee::Sprite>(bg_sprites));
	for (int i = 0, n = bg_sprites.size(); i < n; ++i) {
		ee::SprPtr bg = bg_sprites[i];
		value["bg"][i]["filepath"] = ee::SymbolPath::GetRelativePath(
			std::dynamic_pointer_cast<ee::Symbol>(bg->GetSymbol()), dir);
		bg->Store(value["bg"][i]);
	}

	for (int i = 0, n = m_oceans.size(); i < n; ++i) {
		FileIO::StoreOceanMesh(m_oceans[i], dir, value["ocean"][i]);
	}
}

void StagePanel::Load(const std::string& dir, const Json::Value& value,
					  ee::LibraryPanel* library, ToolbarPanel* toolbar)
{
	int i = 0;
	Json::Value bg_val = value["bg"][i++];
	while (!bg_val.isNull()) {
		std::string filepath = dir + "\\" + bg_val["filepath"].asString();
		auto sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
		ee::SprPtr bg = ee::SpriteFactory::Instance()->Create(sym);
		bg->Load(bg_val);
		ee::InsertSpriteSJ::Instance()->Insert(bg);

		bg_val = value["bg"][i++];
	}

	i = 0;
	Json::Value ocean_val = value["ocean"][i++];
	while (!ocean_val.isNull()) {
		OceanMesh* ocean = FileIO::LoadOceanMesh(dir, ocean_val);
		if (ocean) {
			m_oceans.push_back(ocean);
			ee::InsertShapeSJ::Instance()->Insert(
				const_cast<eshape::PolygonShape*>(ocean->GetBounding()));
			library->AddSymbol(const_cast<ee::ImageSymbol*>(ocean->GetImage0()));
			if (const ee::SymPtr& tex1 = ocean->GetImage1()) {
				library->AddSymbol(const_cast<ee::Symbol*>(tex1));
			}
			toolbar->SetControlersValue(ocean);
		}
		ocean_val = value["ocean"][i++];
	}
}

// const std::vector<OceanMesh*>& StagePanel::GetOceans() const 
// {
// 	return m_sym->GetOceans();
// }
// 
// std::vector<OceanMesh*>& StagePanel::GetOceans() 
// {
// 	return m_sym->GetOceans();
// }

void StagePanel::AddOcean(const eshape::PolygonShape* shape, const std::shared_ptr<ee::ImageSymbol>& image)
{
	bool is_new = true;
	std::vector<OceanMesh*>::iterator itr = m_oceans.begin();
	for ( ; itr != m_oceans.end(); ) {
		OceanMesh* mesh = *itr;
		if (mesh->GetShape() == shape) {
			if (mesh->IsBlendOpen()) {
				is_new = false;
				mesh->SetImage1(image);
				++itr;
			} else {
				delete mesh;
				itr = m_oceans.erase(itr);
			}
		} else {
			++itr;
		}
	}

	if (is_new) {
		m_oceans.push_back(new OceanMesh(shape, image));
	}
}

//////////////////////////////////////////////////////////////////////////
// class StagePanel::StageDropTarget
//////////////////////////////////////////////////////////////////////////

StagePanel::StageDropTarget::
StageDropTarget(StagePanel* stage, ee::LibraryPanel* library)
	: ee::StageDropTarget(stage, stage->GetStageImpl(), library)
	, m_stage(stage)
{
}

bool StagePanel::StageDropTarget::
OnDropSymbol(const ee::SymPtr& sym, const sm::vec2& pos)
{
	if (ee::ImageSymbol* image = dynamic_cast<ee::ImageSymbol*>(sym))
	{
		ee::Shape* shape = m_stage->QueryShapeByPos(pos);
		if (eshape::PolygonShape* poly = dynamic_cast<eshape::PolygonShape*>(shape)) {
			poly->SetMaterialTexture(image);
			m_stage->AddOcean(poly, image);
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		} else {
			auto spr = ee::SpriteFactory::Instance()->Create(sym);
			spr->Translate(pos);
			ee::InsertSpriteSJ::Instance()->Insert(spr);
		}
		return true;
	}

	return false;
}

}