#include "FileIO.h"

#include "dataset/Layer.h"
#include "dataset/LayerFactory.h"
#include "frame/SettingCfg.h"
#include "view/LibraryPanel.h"
#include "view/LibraryPage.h"
#include "view/StagePanel.h"

#include <ee/CameraMgr.h>
#include <ee/OrthoCamera.h>
#include <ee/panel_msg.h>
#include <ee/trans_color.h>
#include <ee/FileHelper.h>
#include <ee/Sprite.h>
#include <ee/Shape.h>

#include <fstream>

namespace lr
{

void FileIO::Load(const char* filename, LibraryPanel* library, 
				  StagePanel* stage, ee::GroupTreePanel* grouptree)
{
// 	ee::SymbolMgr::Instance()->Clear();
// 	ee::BitmapMgr::Instance()->Clear();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	SettingCfg* cfg = SettingCfg::Instance();

	// settings
	if (!value["settings"].isNull()) {
		cfg->m_terrain2d_anim = value["settings"]["terrain2d"].asBool();
	}

	// size
	cfg->m_map_width = value["size"]["width"].asInt();
	cfg->m_map_height = value["size"]["height"].asInt();
	if (value["size"]["view width"].isNull()) {
		cfg->m_view_width = cfg->m_map_width;
		cfg->m_view_height = cfg->m_map_height;
	} else {
		cfg->m_view_dx = value["size"]["view offset x"].asInt();
		cfg->m_view_dy = value["size"]["view offset y"].asInt();
		cfg->m_view_width = value["size"]["view width"].asInt();
		cfg->m_view_height = value["size"]["view height"].asInt();
	}
	stage->BuildGrids(cfg->m_map_width, cfg->m_map_height);

	// camera
	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) 
	{
		float s = value["camera"]["scale"].asDouble();
		float x = value["camera"]["x"].asDouble(),
			  y = value["camera"]["y"].asDouble();
		ee::OrthoCamera* cam = static_cast<ee::OrthoCamera*>(ee::CameraMgr::Instance()->GetCamera());
		cam->SetScale(s);
		cam->SetPosition(ee::Vector(x, y));
		cam->UpdateModelView();
	}

	// screen
	if (!value["screen"]["multi_col"].isNull()) {
		std::string str = value["screen"]["multi_col"].asString();
		stage->GetScreenMultiColor() = ee::TransColor(str, ee::PT_RGBA);
	}
	if (!value["screen"]["add_col"].isNull()) {
		std::string str = value["screen"]["add_col"].asString();
		stage->GetScreenAddColor() = ee::TransColor(str, ee::PT_RGBA);
	}

	// layers
	std::string dir = ee::FileHelper::GetFileDir(filename);
	LoadLayers(value["layer"], stage, library, dir);

// 	// groups
// 	grouptree->LoadFromFile(value["group"]);

	// libraries
	if (value["library"].isNull()) {
		library->LoadSymbolFromLayer();
	} else {
		library->LoadFromFile(value["library"], dir);
	}

	library->Refresh();

	Layer* layer = static_cast<LibraryPage*>(library->GetCurrPage())->GetLayer();
	ee::ChangeLayerMgrSJ::Instance()->Change(layer->GetLayerMgr());
}

void FileIO::Store(const char* filename, LibraryPanel* library,
				   StagePanel* stage, ee::GroupTreePanel* grouptree)
{
	Json::Value value;

	SettingCfg* cfg = SettingCfg::Instance();

	// settings
	value["settings"]["terrain2d"] = cfg->m_terrain2d_anim;

	// size
	value["size"]["width"] = cfg->m_map_width;
	value["size"]["height"] = cfg->m_map_height;
	value["size"]["view width"] = cfg->m_view_width;
	value["size"]["view height"] = cfg->m_view_height;
	value["size"]["view offset x"] = cfg->m_view_dx;
	value["size"]["view offset y"] = cfg->m_view_dy;

	// camera
	ee::Camera* cam = ee::CameraMgr::Instance()->GetCamera();
	value["camera"]["scale"] = cam->GetScale();
	value["camera"]["x"] = cam->GetPosition().x;
	value["camera"]["y"] = cam->GetPosition().y;

	// screen
	value["screen"]["multi_col"] = ee::TransColor(stage->GetScreenMultiColor(), ee::PT_RGBA);
	value["screen"]["add_col"] = ee::TransColor(stage->GetScreenAddColor(), ee::PT_RGBA);

	// layers
	std::string dir = ee::FileHelper::GetFileDir(filename) + "\\";
	StoreLayers(value["layer"], stage->GetLayers(), dir);

// 	// groups
// 	grouptree->StoreToFile(value["group"]);

	// libraries
	library->StoreToFile(value["library"], dir);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileIO::LoadLayers(const Json::Value& value, StagePanel* stage, 
						LibraryPanel* library, const std::string& dir)
{
	std::vector<Layer*> layers;

	int idx = 0;
	Json::Value layer_val = value[idx++];
	while (!layer_val.isNull()) {
		LayerType type = library->GetLayerType(idx-1);
		Layer* layer = LayerFactory::Create(idx-1, library, type, idx != 1);
		layer->LoadFromFile(layer_val, dir, idx-1);
		layers.push_back(layer);

		layer_val = value[idx++];
	}

	for (int i = 1, n = layers.size(); i < n; ++i) {
		Layer* layer = layers[i];
		layer->SetEditable(false);
	}

	stage->SetLayers(layers);
	library->InitFromLayers(layers);
}

void FileIO::StoreLayers(Json::Value& value, const std::vector<Layer*>& layers, const std::string& dir)
{
	for (int i = 0, n = layers.size(); i < n; ++i) {
		layers[i]->StoreToFile(value[i], dir);
	}
}

}