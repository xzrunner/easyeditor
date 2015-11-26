#include "StagePanel.h"
#include "StageCanvas.h"
#include "OceanMesh.h"
#include "FileIO.h"
#include "ToolBarPanel.h"
#include "Sprite.h"

#include <easyshape.h>

namespace eterrain2d
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(GetStageImpl(), library)
	, d2d::ShapesPanelImpl()
{
	SetEditOP(new d2d::ZoomViewOP(this, GetStageImpl(), true));
	SetCanvas(new StageCanvas(this));

 	SetDropTarget(new StageDropTarget(this, library));
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::ISprite* edited, const d2d::MultiSpritesImpl* bg_sprites, 
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(GetStageImpl(), library)
	, d2d::ShapesPanelImpl()
{
	SetEditOP(new d2d::ZoomViewOP(this, GetStageImpl(), true));
	SetCanvas(new StageCanvas(this, edited, bg_sprites));

	m_oceans = static_cast<Sprite*>(edited)->GetSymbol().GetOceans();
	for (int i = 0, n = m_oceans.size(); i < n; ++i) {
		OceanMesh* ocean = m_oceans[i];
		ocean->Retain();
		d2d::InsertShapeSJ::Instance()->Insert(
			const_cast<libshape::PolygonShape*>(ocean->GetBounding()));
	}

	SetDropTarget(new StageDropTarget(this, library));
}

StagePanel::~StagePanel()
{	
	for (int i = 0, n = m_oceans.size(); i < n; ++i) {
		m_oceans[i]->Release();
	}
}

void StagePanel::Store(const std::string& dir, Json::Value& value) const
{
	std::vector<d2d::ISprite*> bg_sprites;
	TraverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(bg_sprites));
	for (int i = 0, n = bg_sprites.size(); i < n; ++i) {
		d2d::ISprite* bg = bg_sprites[i];
		value["bg"][i]["filepath"] = d2d::FilenameTools::getRelativePath(
			dir, bg->GetSymbol().GetFilepath()).ToStdString();
		bg->Store(value["bg"][i]);
	}

	for (int i = 0, n = m_oceans.size(); i < n; ++i) {
		FileIO::StoreOceanMesh(m_oceans[i], dir, value["ocean"][i]);
	}
}

void StagePanel::Load(const std::string& dir, const Json::Value& value,
					  d2d::LibraryPanel* library, ToolbarPanel* toolbar)
{
	int i = 0;
	Json::Value bg_val = value["bg"][i++];
	while (!bg_val.isNull()) {
		std::string filepath = dir + "\\" + bg_val["filepath"].asString();
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
		d2d::ISprite* bg = d2d::SpriteFactory::Instance()->create(symbol);
		bg->Load(bg_val);
		d2d::InsertSpriteSJ::Instance()->Insert(bg);
		symbol->Release();

		bg_val = value["bg"][i++];
	}

	i = 0;
	Json::Value ocean_val = value["ocean"][i++];
	while (!ocean_val.isNull()) {
		OceanMesh* ocean = FileIO::LoadOceanMesh(dir, ocean_val);
		if (ocean) {
			m_oceans.push_back(ocean);
			d2d::InsertShapeSJ::Instance()->Insert(
				const_cast<libshape::PolygonShape*>(ocean->GetBounding()));
			library->AddSymbol(const_cast<d2d::ImageSymbol*>(ocean->GetImage0()));
			if (const d2d::ISymbol* tex1 = ocean->GetImage1()) {
				library->AddSymbol(const_cast<d2d::ISymbol*>(tex1));
			}
			toolbar->SetControlersValue(ocean);
		}
		ocean_val = value["ocean"][i++];
	}
}

// const std::vector<OceanMesh*>& StagePanel::GetOceans() const 
// {
// 	return m_symbol->GetOceans();
// }
// 
// std::vector<OceanMesh*>& StagePanel::GetOceans() 
// {
// 	return m_symbol->GetOceans();
// }

void StagePanel::AddOcean(const libshape::PolygonShape* shape, const d2d::ImageSymbol* image)
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
StageDropTarget(StagePanel* stage, d2d::LibraryPanel* library)
	: d2d::StageDropTarget(stage, NULL, library)
	, m_stage(stage)
{
}

bool StagePanel::StageDropTarget::
OnDropSymbol(d2d::ISymbol* symbol, const d2d::Vector& pos)
{
	if (d2d::ImageSymbol* image = dynamic_cast<d2d::ImageSymbol*>(symbol))
	{
		d2d::IShape* shape = m_stage->QueryShapeByPos(pos);
		if (libshape::PolygonShape* poly = dynamic_cast<libshape::PolygonShape*>(shape)) {
			poly->SetMaterialTexture(image);
			m_stage->AddOcean(poly, image);
		} else {
			d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
			sprite->Translate(pos);
			d2d::InsertSpriteSJ::Instance()->Insert(sprite);
		}
		return true;
	}

	return false;
}

}