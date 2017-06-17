#include "JsonResParser.h"
#include "Database.h"

#include <ee/SymbolFile.h>
#include <ee/SymbolType.h>

#include <sprite2/SymType.h>
#include <gum/FilepathHelper.h>
#include <gum/StringHelper.h>

#include <fstream>

#include <assert.h>

namespace edb
{

static const std::string KEY_SPR = "sprite";
static const std::string KEY_COMP = "components";
static const std::string KEY_PATH = "filepath";
static const std::string KEY_EXPORT_NAME = "name";

void JsonResParser::Parse(const Database& db, const std::string& filepath,
						  std::string& export_name, std::set<int>& output)
{
	output.clear();

	std::string absolute_path = gum::FilepathHelper::Absolute(db.GetDirPath(), filepath);
	int type = ee::SymbolFile::Instance()->Type(absolute_path);
	switch (type)
	{
	case s2::SYM_IMAGE:
		break;
	case s2::SYM_SCALE9:
		ParserCommon(db, filepath, KEY_SPR, output, export_name);
		break;
	case s2::SYM_ICON:
		break;
	case s2::SYM_TEXTURE:
		ParserTexture(db, filepath, output);
		break;
	case s2::SYM_COMPLEX:
		ParserCommon(db, filepath, KEY_SPR, output, export_name);
		break;
	case s2::SYM_ANIMATION:
		ParserAnim(db, filepath, output, export_name);
		break;
	case s2::SYM_ANIM2:
		// todo
		break;
	case s2::SYM_PARTICLE3D:
		ParserCommon(db, filepath, KEY_COMP, output, export_name);
		break;
	case s2::SYM_PARTICLE2D:
		ParserCommon(db, filepath, KEY_COMP, output, export_name);
		break;
	case s2::SYM_SHAPE:
		// todo
		break;
	case s2::SYM_MESH:
		ParserMesh(db, filepath, output);
		break;
	case s2::SYM_MASK:
		ParserMask(db, filepath, output);
		break;
	case s2::SYM_TRAIL:
		// todo		
		break;
	case s2::SYM_SKELETON:
		// todo		
		break;
	case s2::SYM_MODEL:
		// todo
		break;
	case ee::SYM_UIWND:
		ParserCommon(db, filepath, KEY_SPR, output, export_name);
		break;
	case ee::SYM_UI:
		ParserUI(db, filepath, output);
		break;
	}
}

void JsonResParser::ParserFile(const Database& db, const std::string& base_dir,
							   const Json::Value& val, const std::string& key, 
							   std::set<int>& output)
{
	std::string filepath = val[key].asString();
	if (filepath == ee::SYM_GROUP_TAG) {
		return ParserGroup(db, base_dir, val, output);
	}

	std::string absolute = gum::FilepathHelper::Absolute(base_dir, filepath);
	absolute = gum::FilepathHelper::Format(absolute);
	int id = db.QueryByPath(absolute);
	output.insert(id);
}

void JsonResParser::ParserGroup(const Database& db, const std::string& base_dir, 
								const Json::Value& val, std::set<int>& output)
{
	for (int i = 0, n = val["group"].size(); i < n; ++i) {
		ParserFile(db, base_dir, val["group"][i], "filepath", output);
	}
}

void JsonResParser::LoadJson(const std::string& filepath, Json::Value& val)
{
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();
}

void JsonResParser::ParserCommon(const Database& db, const std::string& filepath, 
								 const std::string& key, std::set<int>& output,
								 std::string& export_name)
{
	Json::Value value;
	LoadJson(filepath, value);

	export_name = value[KEY_EXPORT_NAME].asString();
	gum::StringHelper::ToLower(export_name);

	std::string dir = gum::FilepathHelper::Dir(filepath);
	int n = value[key].size();
	for (int i = 0; i < n; ++i) {
		ParserFile(db, dir, value[key][i], KEY_PATH, output);
	}
}

void JsonResParser::ParserTexture(const Database& db, const std::string& filepath, 
								  std::set<int>& output)
{
	Json::Value value;
	LoadJson(filepath, value);

	std::string dir = gum::FilepathHelper::Dir(filepath);
	for (int i = 0, n = value["shapes"].size(); i < n; ++i) 
	{
		if (!value["shapes"][i].isMember("material")) {
			continue;
		}
		const Json::Value& val_mat = value["shapes"][i]["material"];
		if (val_mat["type"] == "texture") {
			ParserFile(db, dir, val_mat, "texture path", output);
		}
	}
}

void JsonResParser::ParserAnim(const Database& db, const std::string& filepath, 
							   std::set<int>& output, std::string& export_name)
{
	Json::Value value;
	LoadJson(filepath, value);

	export_name = value[KEY_EXPORT_NAME].asString();

	std::string dir = gum::FilepathHelper::Dir(filepath);
	for (int layer_i = 0, layer_n = value["layer"].size(); layer_i < layer_n; ++layer_i) {
		Json::Value& layer_val = value["layer"][layer_i];
		for (int frame_i = 0, frame_n = layer_val["frame"].size(); frame_i < frame_n; ++frame_i) {
			Json::Value& frame_val = layer_val["frame"][frame_i];
			for (int actor_i = 0, actor_n = frame_val["actor"].size(); actor_i < actor_n; ++actor_i) {
				ParserFile(db, dir, frame_val["actor"][actor_i], KEY_PATH, output);
			}
		}
	}
}

void JsonResParser::ParserMesh(const Database& db, const std::string& filepath,
							   std::set<int>& output)
{
	Json::Value value;
	LoadJson(filepath, value);

	std::string dir = gum::FilepathHelper::Dir(filepath);
	ParserFile(db, dir, value, "base_symbol", output);
}

void JsonResParser::ParserMask(const Database& db, const std::string& filepath,
							   std::set<int>& output)
{
	Json::Value value;
	LoadJson(filepath, value);

	std::string dir = gum::FilepathHelper::Dir(filepath);
	ParserFile(db, dir, value["base"], KEY_PATH, output);
	ParserFile(db, dir, value["mask"], KEY_PATH, output);
}

void JsonResParser::ParserUI(const Database& db, const std::string& filepath, std::set<int>& output)
{
	Json::Value value;
	LoadJson(filepath, value);

	std::string dir = gum::FilepathHelper::Dir(filepath);
	ParserFile(db, dir, value, "items filepath", output);
	ParserFile(db, dir, value, "wrapper filepath", output);
}

}