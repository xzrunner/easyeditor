#include "FileIO.h"

#include "dataset/Layer.h"
#include "frame/SettingCfg.h"
#include "view/LibraryPanel.h"
#include "view/StagePanel.h"

namespace lr
{

void FileIO::Load(const char* filename, StagePanel* stage,
				  LibraryPanel* library, d2d::GroupTreePanel* grouptree)
{
// 	d2d::SymbolMgr::Instance()->clear();
// 	d2d::BitmapMgr::Instance()->clear();

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
		cfg->m_view_width = value["size"]["view width"].asInt();
		cfg->m_view_height = value["size"]["view height"].asInt();
	}
	stage->BuildGrids(cfg->m_map_width, cfg->m_map_height);

	// camera
	float s = value["camera"]["scale"].asDouble();
	float x = value["camera"]["x"].asDouble(),
		y = value["camera"]["y"].asDouble();
	d2d::Camera* cam = stage->getCamera();
	cam->SetScale(s);
	cam->SetPosition(d2d::Vector(x, y));

	// layers
	std::string dir = d2d::FilenameTools::getFileDir(filename);
	LoadLayers(value["layer"], stage, library, dir);
	library->RefreshViewList();

	// groups
	grouptree->LoadFromFile(value["group"]);
}

void FileIO::Store(const char* filename, StagePanel* stage, 
				   d2d::GroupTreePanel* grouptree)
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

	// camera
	d2d::Camera* cam = stage->getCamera();
	value["camera"]["scale"] = cam->GetScale();
	value["camera"]["x"] = cam->GetPosition().x;
	value["camera"]["y"] = cam->GetPosition().y;

	// layers
	std::string dir = d2d::FilenameTools::getFileDir(filename) + "\\";
	StoreLayers(value["layer"], stage->GetLayers(), dir);

	// groups
	grouptree->StoreToFile(value["group"]);

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
		Layer* layer = new Layer;
		layer->LoadFromFile(layer_val, dir, idx-1);
		layers.push_back(layer);

		layer_val = value[idx++];
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