#include "JsonRefParser.h"
#include "Database.h"

#include <ee/SymbolFile.h>
#include <ee/SymbolType.h>

#include <sprite2/SymType.h>
#include <gum/FilepathHelper.h>

#include <fstream>

#include <assert.h>

namespace edb
{

static const std::string KEY_SPR = "sprite";
static const std::string KEY_COMP = "components";
static const std::string KEY_PATH = "filepath";

void JsonRefParser::GetOutput(const Database& db, const std::string& filepath,
							  std::set<int>& output)
{
	output.clear();

	std::string absolute_path = gum::FilepathHelper::Absolute(db.GetDirPath(), filepath);
	int type = ee::SymbolFile::Instance()->Type(absolute_path);
	switch (type)
	{
	case s2::SYM_IMAGE:
		break;
	case s2::SYM_SCALE9:
		ParserCommonArray(db, filepath, KEY_SPR, output);
		break;
	case s2::SYM_ICON:
		break;
	case s2::SYM_TEXTURE:
		ParserTexture(db, filepath, output);
		break;
	case s2::SYM_COMPLEX:
		ParserCommonArray(db, filepath, KEY_SPR, output);
		break;
	case s2::SYM_ANIMATION:
		ParserAnim(db, filepath, output);
		break;
	case s2::SYM_ANIM2:
		// todo
		break;
	case s2::SYM_PARTICLE3D:
		ParserCommonArray(db, filepath, KEY_COMP, output);
		break;
	case s2::SYM_PARTICLE2D:
		ParserCommonArray(db, filepath, KEY_COMP, output);
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
	}
}

void JsonRefParser::ParserFile(const Database& db, const std::string& base_dir,
							   const Json::Value& val, const std::string& key, 
							   std::set<int>& output)
{
	std::string filepath = val[key].asString();
	if (filepath == ee::SYM_GROUP_TAG) {
		return ParserGroup(db, base_dir, val, output);
	}

	std::string absolute = gum::FilepathHelper::Absolute(base_dir, filepath);
	absolute = gum::FilepathHelper::Format(absolute);
	int id = db.Query(absolute);
	assert(id != -1);
	output.insert(id);
}

void JsonRefParser::ParserGroup(const Database& db, const std::string& base_dir, 
								const Json::Value& val, std::set<int>& output)
{
	for (int i = 0, n = val["group"].size(); i < n; ++i) {
		ParserFile(db, base_dir, val["group"][i], "filepath", output);
	}
}

void JsonRefParser::LoadJson(const std::string& filepath, Json::Value& val)
{
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();
}

void JsonRefParser::ParserCommonArray(const Database& db, const std::string& filepath, 
									  const std::string& key, std::set<int>& output)
{
	Json::Value value;
	LoadJson(filepath, value);

	std::string dir = gum::FilepathHelper::Dir(filepath);
	int n = value[KEY_SPR].size();
	for (int i = 0; i < n; ++i) {
		ParserFile(db, dir, value[key][i], KEY_PATH, output);
	}
}

void JsonRefParser::ParserTexture(const Database& db, const std::string& filepath, 
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

void JsonRefParser::ParserAnim(const Database& db, const std::string& filepath, 
							   std::set<int>& output)
{
	Json::Value value;
	LoadJson(filepath, value);

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

void JsonRefParser::ParserMesh(const Database& db, const std::string& filepath,
							   std::set<int>& output)
{
	Json::Value value;
	LoadJson(filepath, value);

	std::string dir = gum::FilepathHelper::Dir(filepath);
	ParserFile(db, dir, value, "base_symbol", output);
}

void JsonRefParser::ParserMask(const Database& db, const std::string& filepath,
							   std::set<int>& output)
{
	Json::Value value;
	LoadJson(filepath, value);

	std::string dir = gum::FilepathHelper::Dir(filepath);
	ParserFile(db, dir, value["base"], KEY_PATH, output);
	ParserFile(db, dir, value["mask"], KEY_PATH, output);
}

}