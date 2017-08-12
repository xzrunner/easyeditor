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
 
 	std::string dir = gum::FilepathHelper::Dir(src_file);
 	gum::BodymovinParser parser;
 	parser.Parse(val, dir);
 
 	ee::SymbolLoader sym_loader;
 	ee::SpriteLoader spr_loader;
 
 	std::map<std::string, s2::Sprite*> map_assets;
 	const std::vector<gum::BodymovinParser::Asset>& assets = parser.GetAssets();
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
 				std::string filepath = gum::FilepathHelper::Absolute(".", a.filepath);
 				s2::Sprite* spr = spr_loader.Create(filepath);
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
 					const std::string& id = a.layers[j].ref_id;
 					std::map<std::string, s2::Sprite*>::iterator itr = map_assets.find(id);
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
 
 				libanim::Symbol* sym = new libanim::Symbol();
 				gum::BodymovinAnimLoader loader(sym, &sym_loader, &spr_loader);
 				loader.LoadLayers(map_assets, a.layers, parser.GetFrameRate(), parser.GetWidth(), 
					parser.GetHeight(), parser.GetStartFrame(), parser.GetEndFrame());
 				std::string filepath = dst_dir + "\\" + a.id + "_" + ee::SymbolFile::Instance()->Tag(s2::SYM_ANIMATION) + ".json";
 				libanim::FileSaver::Store(filepath, *sym);
 
 				sym->SetFilepath(filepath);
 				libanim::Sprite* spr = new libanim::Sprite(sym);
 				spr->UpdateBounding();
 				map_assets.insert(std::make_pair(a.id, spr));
 
 				sym->RemoveReference();
 			}
 		}
 		if (!fail) {
 			break;
 		}
 	}
 
 	libanim::Symbol* sym = new libanim::Symbol();
 	gum::BodymovinAnimLoader loader(sym, &sym_loader, &spr_loader);
 	loader.LoadLayers(map_assets, parser.GetLayers(), parser.GetFrameRate(), parser.GetWidth(), 
		parser.GetHeight(), parser.GetStartFrame(), parser.GetEndFrame());
 	std::string filepath = dst_dir + "\\data_" + ee::SymbolFile::Instance()->Tag(s2::SYM_ANIMATION) + ".json";
 	libanim::FileSaver::Store(filepath, *sym);
 
 	std::map<std::string, s2::Sprite*>::iterator itr = map_assets.begin();
 	for ( ; itr != map_assets.end(); ++itr) {
 		itr->second->RemoveReference();
 	}

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

	std::string layer_name = gum::FilepathHelper::Filename(filepath);
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
		filename = gum::FilepathHelper::Filename(filename);

		int sz = dst_val["layer"].size();
		dst_val["layer"][sz] = val["layer"][i];
		if (filename.size() != 6 ||
			filename[0] != '0' ||
			filename.substr(2) != ".png") 
		{
			continue;
		}

		ee::Symbol* t_sym = ee::SymbolFactory::Create(s2::SYM_TEXTBOX);
		s2::Textbox tb;
		tb.width = 200;
		tb.height = 200;
		tb.font_size = 40;
		tb.font_color = s2::Color(0, 0, 0);
		tb.has_edge = false;
		tb.align_hori = s2::Textbox::HA_LEFT;
		tb.align_vert = s2::Textbox::VA_TOP;
		dynamic_cast<etext::Symbol*>(t_sym)->SetTextbox(tb);
		s2::Sprite* t_spr = ee::SpriteFactory::Instance()->Create(t_sym);
		t_spr->UpdateBounding();

		ee::Symbol* c_sym = ee::SymbolFactory::Create(s2::SYM_COMPLEX);
		dynamic_cast<ecomplex::Symbol*>(c_sym)->Add(t_spr);
		std::string text_path = layer_name + "_" + gum::StringHelper::ToString(i) + "_text_complex.json";
		c_sym->SetFilepath(dir + "\\" + text_path);
		s2::Sprite* c_spr = ee::SpriteFactory::Instance()->Create(c_sym);
		c_spr->UpdateBounding();

		ecomplex::FileStorer::Store(c_sym->GetFilepath(), dynamic_cast<ecomplex::Symbol*>(c_sym), dir);

		Json::Value new_layer = layer_val;
		for (int j = 0, m = new_layer["frame"].size(); j < m; ++j)
		{
			Json::Value& frame_val = new_layer["frame"][j];
			assert(frame_val["actor"].size() == 1);
			const Json::Value& src_val = frame_val["actor"][IDX0];

			ee::SpriteIO spr_io;
			spr_io.Load(src_val, dir);

			sm::vec2 anchor = spr_io.m_position + spr_io.m_offset;

			spr_io.m_position = spr_io.m_position + sm::rotate_vector(-spr_io.m_offset, spr_io.m_angle) + spr_io.m_offset;
			spr_io.m_angle = 0;
			float scale = std::min(fabs(spr_io.m_scale.x), fabs(spr_io.m_scale.y));
			spr_io.m_scale.x = scale;
			spr_io.m_scale.y = scale;
			spr_io.m_offset = anchor - spr_io.m_position;

			Json::Value dst_val;
			dst_val["filepath"] = text_path;
			spr_io.Store(dst_val, dir);

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