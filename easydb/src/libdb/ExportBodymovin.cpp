#include "ExportBodymovin.h"
#include "check_params.h"
#include "utility.h"

#include <ee/SpriteLoader.h>
#include <ee/SymbolLoader.h>
#include <ee/SymbolFile.h>
#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>
#include <ee/FileHelper.h>
#include <ee/SpriteIO.h>

#include <easyanim.h>
#include <easycomplex.h>
#include <easytext.h>

#include <sprite2/TextboxSymbol.h>
#include <sprite2/ComplexSymbol.h>
#include <sprite2/SprSRT.h>
#include <gum/FilepathHelper.h>
#include <gum/BodymovinAnimLoader.h>
#include <gum/BodymovinParser.h>
#include <gum/StringHelper.h>

#include <fstream>

namespace edb
{

std::string ExportBodymovin::Command() const
{
	return "export-bodymovin";
}

std::string ExportBodymovin::Description() const
{
	return "export-bodymovin";
}

std::string ExportBodymovin::Usage() const
{
	std::string usage = Command() + " [src file] [dst dir]";
	return usage;
}

int ExportBodymovin::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return -1;
	if (!check_folder(argv[3])) return -1;

	int ret = init_gl();
	if (ret < 0) {
		return ret;
	}

	Trigger(argv[2], argv[3]);

	return 0;
}

void ExportBodymovin::Trigger(const std::string& src_file, const std::string& dst_dir)
{
 	Json::Value val;
 	Json::Reader reader;
 	std::locale::global(std::locale(""));
 	std::ifstream fin(src_file.c_str());
 	std::locale::global(std::locale("C"));
 	reader.parse(fin, val);
 	fin.close();
 
 	auto dir = gum::FilepathHelper::Dir(src_file.c_str());
 	gum::BodymovinParser parser;
 	parser.Parse(val, dir);
 
 	auto sym_loader = std::make_shared<ee::SymbolLoader>();
 	auto spr_loader = std::make_shared<ee::SpriteLoader>();
 
 	CU_MAP<CU_STR, s2::SprPtr> map_assets;
 	auto& assets = parser.GetAssets();
 	std::vector<bool> flags(assets.size(), false);
 	while (true)
 	{
 		bool fail = false;
 		for (int i = 0, n = assets.size(); i < n; ++i)
 		{
 			if (flags[i]) {
 				continue;
 			}			
 			const gum::BodymovinParser::Asset& a = assets[i];
 			if (a.layers.empty()) 
 			{
 				auto filepath = gum::FilepathHelper::Absolute(".", a.filepath);
 				auto spr = spr_loader->Create(filepath);
 				map_assets.insert(std::make_pair(a.id, spr));
 				flags[i] = true;
 			}
 			else
 			{
 				bool skip = false;
 				for (int j = 0, m = a.layers.size(); j < m; ++j) 
 				{
 					const gum::BodymovinParser::Layer& layer = a.layers[j];
 					if (layer.layer_type == gum::BodymovinParser::LAYER_SOLID ||
						layer.layer_type == gum::BodymovinParser::LAYER_NULL) {
 						continue;
 					}
 					auto& id = a.layers[j].ref_id;
 					auto itr = map_assets.find(id);
 					if (itr == map_assets.end()) {
 						skip = true;
 						break;
 					}
 				}
 				flags[i] = !skip;
 				if (skip) {
 					fail = true;
 					continue;
 				}
 
 				auto sym = std::make_shared<libanim::Symbol>();
 				gum::BodymovinAnimLoader loader(*std::dynamic_pointer_cast<s2::AnimSymbol>(sym), sym_loader, spr_loader);
 				loader.LoadLayers(map_assets, a.layers, parser.GetFrameRate(), parser.GetWidth(), 
					parser.GetHeight(), parser.GetStartFrame(), parser.GetEndFrame());
 				std::string filepath = dst_dir + "\\" + std::string(a.id.c_str()) + "_" + ee::SymbolFile::Instance()->Tag(s2::SYM_ANIMATION) + ".json";
 				libanim::FileSaver::Store(filepath, *sym);
 
 				sym->SetFilepath(filepath);
 				libanim::Sprite* spr = new libanim::Sprite(sym);
 				spr->UpdateBounding();
 				map_assets.insert(std::make_pair(a.id, spr));
 			}
 		}
 		if (!fail) {
 			break;
 		}
 	}
 
 	auto sym = std::make_shared<libanim::Symbol>();
 	gum::BodymovinAnimLoader loader(*sym, sym_loader, spr_loader);
 	loader.LoadLayers(map_assets, parser.GetLayers(), parser.GetFrameRate(), parser.GetWidth(), 
		parser.GetHeight(), parser.GetStartFrame(), parser.GetEndFrame());
 	std::string filepath = dst_dir + "\\data_" + ee::SymbolFile::Instance()->Tag(s2::SYM_ANIMATION) + ".json";
 	libanim::FileSaver::Store(filepath, *sym);
 
	FixFontLayers(dst_dir);
}

void ExportBodymovin::FixFontLayers(const std::string& dir)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath();
		if (ee::SymbolFile::Instance()->Type(filepath) == s2::SYM_ANIMATION) {
			FixFontLayer(filepath, dir);
		}
	}
}

void ExportBodymovin::FixFontLayer(const std::string& filepath, const std::string& dir)
{
	Json::Value val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	Json::Value dst_val;
	dst_val["fps"] = val["fps"];
	dst_val["name"] = val["name"];

	int IDX0 = 0;

	auto layer_name = gum::FilepathHelper::Filename(filepath.c_str());
	layer_name = layer_name.substr(0, layer_name.size() - 10);

	bool dirty = false;
	for (int i = 0, n = val["layer"].size(); i < n; ++i)
	{
		const Json::Value& layer_val = val["layer"][i];
		assert(layer_val.size() > 0 && layer_val["frame"].size() > 0);
		const Json::Value& frame_val = layer_val["frame"][IDX0];
		assert(frame_val.size() > 0 && frame_val["actor"].size() > 0);
		const Json::Value& actor_val = frame_val["actor"][IDX0];
		std::string filename = actor_val["filepath"].asString();
		filename = gum::FilepathHelper::Filename(filename.c_str()).c_str();

		int sz = dst_val["layer"].size();
		dst_val["layer"][sz] = val["layer"][i];
		if (filename.size() != 6 ||
			filename[0] != '0' ||
			filename.substr(2) != ".png") 
		{
			continue;
		}

		auto t_sym = ee::SymbolFactory::Create(s2::SYM_TEXTBOX);
		s2::Textbox tb;
		tb.width = 200;
		tb.height = 200;
		tb.font_type = 0;
		tb.font_size = 40;
		tb.font_color = s2::Color(0, 0, 0);
		tb.has_edge = false;
		tb.align_hori = s2::Textbox::HA_LEFT;
		tb.align_vert = s2::Textbox::VA_CENTER;
		std::dynamic_pointer_cast<etext::Symbol>(t_sym)->SetTextbox(tb);
		auto t_spr = ee::SpriteFactory::Instance()->Create(t_sym);
		t_spr->UpdateBounding();

		auto text_spr = std::dynamic_pointer_cast<etext::Sprite>(t_spr);
		text_spr->SetExport(true);

		auto c_sym = ee::SymbolFactory::Create(s2::SYM_COMPLEX);
		std::dynamic_pointer_cast<ecomplex::Symbol>(c_sym)->Add(t_spr);
		CU_STR text_path = layer_name + "_" + gum::StringHelper::ToString(i) + "_text_complex.json";
		c_sym->SetFilepath(dir + "\\" + std::string(text_path.c_str()));
		auto c_spr = ee::SpriteFactory::Instance()->Create(c_sym);
		c_spr->UpdateBounding();

		ecomplex::FileStorer::Store(c_sym->GetFilepath(), *std::dynamic_pointer_cast<ecomplex::Symbol>(c_sym), dir, false);

		Json::Value new_layer = layer_val;
		for (int j = 0, m = new_layer["frame"].size(); j < m; ++j)
		{
			Json::Value& frame_val = new_layer["frame"][j];
			assert(frame_val["actor"].size() == 1);
			const Json::Value& src_val = frame_val["actor"][IDX0];

			ee::SpriteIO spr_io;
			spr_io.Load(src_val, dir.c_str());

			sm::vec2 anchor = spr_io.m_position + spr_io.m_offset;

			spr_io.m_position = spr_io.m_position + sm::rotate_vector(-spr_io.m_offset, spr_io.m_angle) + spr_io.m_offset;
			spr_io.m_angle = 0;
			float scale = std::min(fabs(spr_io.m_scale.x), fabs(spr_io.m_scale.y));
			spr_io.m_scale.x = scale;
			spr_io.m_scale.y = scale;
			spr_io.m_offset = anchor - spr_io.m_position;

			Json::Value dst_val;
			dst_val["filepath"] = text_path.c_str();
			spr_io.Store(dst_val, dir.c_str());

			frame_val["actor"][IDX0] = dst_val;
		}

		dst_val["layer"][sz + 1] = new_layer;

		dirty = true;
	}

	if (dirty) 
	{
		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(filepath.c_str());
		std::locale::global(std::locale("C"));
		writer.write(fout, dst_val);
		fout.close();
	}
}

}