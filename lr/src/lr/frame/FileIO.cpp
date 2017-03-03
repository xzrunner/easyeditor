#include "FileIO.h"

#include "dataset/Layer.h"
#include "dataset/LayerFactory.h"
#include "frame/SettingCfg.h"
#include "view/LibraryPanel.h"
#include "view/LibraryPage.h"
#include "view/StagePanel.h"
#include "view/StageCanvas.h"

#include <ee/panel_msg.h>
#include <ee/FileHelper.h>
#include <ee/Sprite.h>
#include <ee/Shape.h>
#include <ee/Image.h>

#include <shaderlab/ShaderMgr.h>
#include <shaderlab/ColGradingProg.h>
#include <shaderlab/FilterShader.h>
#include <gum/trans_color.h>
#include <sprite2/OrthoCamera.h>
#include <gum/Config.h>

#include <fstream>

namespace lr
{

void FileIO::Load(const char* filename, LibraryPanel* library, 
				  StagePanel* stage, ee::GroupTreePanel* grouptree)
{
// 	ee::SymbolMgr::Instance()->Clear();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	SettingCfg* cfg = SettingCfg::Instance();

	std::string dir = ee::FileHelper::GetFileDir(filename);

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
	ee::CameraCanvas* canvas = static_cast<ee::CameraCanvas*>(stage->GetCanvas());
	if (canvas->GetCamera()->Type() == s2::CAM_ORTHO2D)
	{
		s2::OrthoCamera* cam = static_cast<s2::OrthoCamera*>(canvas->GetCamera());
		float s = value["camera"]["scale"].asDouble();
		float x = value["camera"]["x"].asDouble(),
			  y = value["camera"]["y"].asDouble();
		cam->SetScale(s);
		cam->SetPosition(sm::vec2(x, y));
	}

	// screen
	if (!value["screen"]["multi_col"].isNull()) {
		std::string str = value["screen"]["multi_col"].asString();
		stage->GetScreenMultiColor() = gum::str2color(str, s2::RGBA);
	}
	if (!value["screen"]["add_col"].isNull()) {
		std::string str = value["screen"]["add_col"].asString();
		s2::Color col = gum::str2color(str, s2::RGBA);
		col.a = 0;
		stage->GetScreenAddColor() = col;
	}
	if (!value["screen"]["post effect"].isNull()) {
		std::string filepath = value["screen"]["post effect"].asString();
		filepath = ee::FileHelper::GetAbsolutePath(dir, filepath);
		
		sl::ColGradingProg* prog = NULL;
		sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
		sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader(sl::FILTER));
		if (shader) {
			prog = static_cast<sl::ColGradingProg*>(shader->GetProgram(sl::FM_COL_GRADING));
		}
		if (prog) 
		{
			gum::Config* cfg = gum::Config::Instance();
			bool ori_alpha_cfg = cfg->GetPreMulAlpha();
			cfg->SetPreMulAlpha(false);
			ee::Image* img = ee::ImageMgr::Instance()->GetItem(filepath);
			cfg->SetPreMulAlpha(ori_alpha_cfg);
			if (img) {
				SettingCfg::Instance()->m_post_effect_file = filepath;
				prog->SetLUTTex(img->GetTexID());
				static_cast<StageCanvas*>(stage->GetCanvas())->EnableColGrading(true);
			}
		}
	}

	// layers
	stage->SetResDir(dir);
	LoadLayers(value["layer"], stage, library, dir);

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

	std::string dir = ee::FileHelper::GetFileDir(filename) + "\\";

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
	ee::CameraCanvas* canvas = static_cast<ee::CameraCanvas*>(stage->GetCanvas());
	if (canvas->GetCamera()->Type() == s2::CAM_ORTHO2D)
	{
		s2::OrthoCamera* cam = static_cast<s2::OrthoCamera*>(canvas->GetCamera());
		value["camera"]["scale"] = cam->GetScale();
		value["camera"]["x"] = cam->GetPosition().x;
		value["camera"]["y"] = cam->GetPosition().y;
	}

	// screen
	value["screen"]["multi_col"] = gum::color2str(stage->GetScreenMultiColor(), s2::RGBA);
	value["screen"]["add_col"]   = gum::color2str(stage->GetScreenAddColor(), s2::RGBA);
	if (!cfg->m_post_effect_file.empty()) {
		value["screen"]["post effect"] = ee::FileHelper::GetRelativePath(dir, cfg->m_post_effect_file);
	}

	// layers
	StoreLayers(value["layer"], stage->GetLayers(), dir);

	// libraries
	library->StoreToFile(value["library"], dir);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();

	wxMessageBox("Success");
}

void FileIO::LoadLayers(const Json::Value& value, StagePanel* stage, 
						LibraryPanel* library, const std::string& dir)
{
	std::vector<Layer*> layers;

	int idx = 0;
	Json::Value layer_val = value[idx++];
	while (!layer_val.isNull()) {
		LayerType type = library->GetLayerType(idx-1);
		Layer* layer = LayerFactory::Create(idx-1, library, type, library->GetLayerCameraMode(idx-1));
		layer->LoadFromFile(layer_val, dir, idx-1);
		layers.push_back(layer);

		layer_val = value[idx++];
	}
	if (layers.empty()) {
		return;
	}

	for (int i = 0, n = layers.size(); i < n; ++i) {
		Layer* layer = layers[i];
		layer->SetEditable(false);
	}

	stage->SetLayers(layers);
	library->InitFromLayers(layers);

	LibraryPage* curr_page = static_cast<LibraryPage*>(library->GetCurrPage());
	curr_page->GetLayer()->SetEditable(true);
	curr_page->UpdateStatusFromLayer();
}

void FileIO::StoreLayers(Json::Value& value, const std::vector<Layer*>& layers, const std::string& dir)
{
	for (int i = 0, n = layers.size(); i < n; ++i) {
		layers[i]->StoreToFile(value[i], dir);
	}
}

}