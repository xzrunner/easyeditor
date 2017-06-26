#include "JsonResOP.h"

#include <ee/SymbolFile.h>
#include <ee/SymbolType.h>

#include <sprite2/SymType.h>
#include <gum/FilepathHelper.h>

#include <fstream>

namespace edb
{

const std::string JsonResOP::KEY_SPR  = "sprite";
const std::string JsonResOP::KEY_COMP = "components";
const std::string JsonResOP::KEY_PATH = "filepath";

JsonResOP::JsonResOP(const std::string& filepath)
	: m_filepath(filepath)
{
	m_base_dir = gum::FilepathHelper::Dir(m_filepath);
}

void JsonResOP::Do()
{
	Clear();

	int type = ee::SymbolFile::Instance()->Type(m_filepath);
	switch (type)
	{
	case s2::SYM_IMAGE:
		break;
	case s2::SYM_SCALE9:
		DoCommon(KEY_SPR);
		break;
	case s2::SYM_ICON:
		break;
	case s2::SYM_TEXTURE:
		DoTexture();
		break;
	case s2::SYM_COMPLEX:
		DoCommon(KEY_SPR);
		break;
	case s2::SYM_ANIMATION:
		DoAnim();
		break;
	case s2::SYM_ANIM2:
		// todo
		break;
	case s2::SYM_PARTICLE3D:
		DoCommon(KEY_COMP);
		break;
	case s2::SYM_PARTICLE2D:
		DoCommon(KEY_COMP);
		break;
	case s2::SYM_SHAPE:
		// todo
		break;
	case s2::SYM_MESH:
		DoMesh();
		break;
	case s2::SYM_MASK:
		DoMask();
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
		DoUIWnd();
		break;
	case ee::SYM_UI:
		DoUI();
		break;
	}
}

bool JsonResOP::DoFile(Json::Value& val, const std::string& key)
{
	std::string filepath = val[key].asString();
	if (filepath == ee::SYM_GROUP_TAG) {
		return DoGroup(val, key);
	} else {
		std::string absolute;
		if (!filepath.empty()) {
			absolute = gum::FilepathHelper::Absolute(m_base_dir, filepath);
			absolute = gum::FilepathHelper::Format(absolute);			
		}
		return OnDoFile(absolute, val, key);
	}
}

bool JsonResOP::DoGroup(Json::Value& val, const std::string& key)
{
	bool dirty = false;
	for (int i = 0, n = val["group"].size(); i < n; ++i) {
		if (DoFile(val["group"][i], "filepath")) {
			dirty = true;
		}
	}
	AfterDoGroup(dirty, val);
	return dirty;
}

void JsonResOP::DoCommon(const std::string& key)
{
	Json::Value value;
	LoadJson(m_filepath, value);

	BeforeDoCommon(value);

	bool dirty = false;
	int n = value[key].size();
	for (int i = 0; i < n; ++i) {
		if (DoFile(value[key][i], KEY_PATH)) {
			dirty = true;
		}
	}

	AfterDoCommon(dirty, value, key);

	if (dirty) {
		StoreJson(m_filepath, value);
	}
}

void JsonResOP::DoUIWnd()
{
	Json::Value value;
	LoadJson(m_filepath, value);

	BeforeDoCommon(value);

	bool dirty = false;
	int n = value[KEY_SPR].size();
	for (int i = 0; i < n; ++i) {
		if (DoFile(value[KEY_SPR][i], KEY_PATH)) {
			dirty = true;
		}
	}

	if (value.isMember("ref_spr")) 
	{
		for (int i = 0, n = value["ref_spr"].size(); i < n; ++i) {
			if (DoFile(value["ref_spr"][i], KEY_PATH)) {
				dirty = true;
			}
		}
	}

	AfterDoCommon(dirty, value, KEY_SPR);

	if (dirty) {
		StoreJson(m_filepath, value);
	}
}

void JsonResOP::DoTexture()
{
	Json::Value value;
	LoadJson(m_filepath, value);

	bool dirty = false;
	for (int i = 0, n = value["shapes"].size(); i < n; ++i) 
	{
		if (!value["shapes"][i].isMember("material")) {
			continue;
		}
		Json::Value& val_mat = value["shapes"][i]["material"];
		if (val_mat["type"] == "texture") {
			if (DoFile(val_mat, "texture path")) {
				dirty = true;
			}
		}
	}

	AfterDoTexture(dirty, value);

	if (dirty) {
		StoreJson(m_filepath, value);
	}
}

void JsonResOP::DoAnim()
{
	Json::Value value;
	LoadJson(m_filepath, value);

	BeforeDoAinm(value);

	bool dirty = false;
	for (int layer_i = 0, layer_n = value["layer"].size(); layer_i < layer_n; ++layer_i) {
		Json::Value& layer_val = value["layer"][layer_i];
		for (int frame_i = 0, frame_n = layer_val["frame"].size(); frame_i < frame_n; ++frame_i) {
			Json::Value& frame_val = layer_val["frame"][frame_i];
			for (int actor_i = 0, actor_n = frame_val["actor"].size(); actor_i < actor_n; ++actor_i) {
				if (DoFile(frame_val["actor"][actor_i], KEY_PATH)) {
					dirty = true;
				}
			}
		}
	}

	AfterDoAnim(dirty, value);

	if (dirty) {
		StoreJson(m_filepath, value);
	}
}

void JsonResOP::DoMesh()
{
	Json::Value value;
	LoadJson(m_filepath, value);

	if (DoFile(value, "base_symbol")) {
		StoreJson(m_filepath, value);
	}
}

void JsonResOP::DoMask()
{
	Json::Value value;
	LoadJson(m_filepath, value);

	bool dirty = false;
	if (DoFile(value["base"], KEY_PATH)) {
		dirty = true;
	}
	if (DoFile(value["mask"], KEY_PATH)) {
		dirty = true;
	}
	if (dirty) {
		StoreJson(m_filepath, value);
	}
}

void JsonResOP::DoUI()
{
	Json::Value value;
	LoadJson(m_filepath, value);

	bool dirty = false;
	if (DoFile(value, "items filepath")) {
		dirty = true;
	}
	if (DoFile(value, "wrapper filepath")) {
		dirty = true;
	}
	if (dirty) {
		StoreJson(m_filepath, value);
	}
}

void JsonResOP::LoadJson(const std::string& filepath, Json::Value& val)
{
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();
}

void JsonResOP::StoreJson(const std::string& filepath, const Json::Value& val)
{
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));
	writer.write(fout, val);
	fout.close();
}

}