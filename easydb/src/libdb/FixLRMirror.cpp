#include "FixLRMirror.h"
#include "FixMirror.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/SymbolType.h>

#include <SM_Vector.h>
#include <sprite2/Color.h>
#include <gum/trans_color.h>

#include <fstream>

namespace edb
{

std::string FixLRMirror::Command() const
{
	return "fix-lr-mirror";
}

std::string FixLRMirror::Description() const
{
	return "fix old LR mirror";
}

std::string FixLRMirror::Usage() const
{
	return Command() + " [old dir] [new dir]";
}

int FixLRMirror::Run(int argc, char *argv[])
{
	// fix-lr-mirror D:\projects\ejoy\coco-tools\sg_particle\data\json_and_pngs

	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;
	if (!check_folder(argv[3])) return -1;

	Trigger(argv[2], argv[3]);

	return 0;
}

void FixLRMirror::Trigger(const std::string& old_dir, const std::string& new_dir) const
{
	wxArrayString old_files, new_files;
	ee::FileHelper::FetchAllFiles(old_dir, old_files);
	ee::FileHelper::FetchAllFiles(new_dir, new_files);
	for (int i = 0, n = old_files.size(); i < n; ++i) 
	{
		std::string old_filepath = ee::FileHelper::GetAbsolutePath(old_files[i].ToStdString());
		if (old_filepath.find("_lr.json") == std::string::npos) {
			continue;
		}

		Json::Value old_val;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(old_filepath.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, old_val);
		fin.close();

		std::string old_name = ee::FileHelper::GetFilename(old_filepath);
		for (int j = 0, m = new_files.size(); j < m; ++j)
		{
			std::string new_filepath = ee::FileHelper::GetAbsolutePath(new_files[j].ToStdString());
			if (new_filepath.find("_lr.json") == std::string::npos) {
				continue;
			}
			if (new_filepath.find(old_name) != std::string::npos) {
				Json::Value new_val;
				Json::Reader reader;
				std::locale::global(std::locale(""));
				std::ifstream fin(new_filepath.c_str());
				std::locale::global(std::locale("C"));
				reader.parse(fin, new_val);
				fin.close();

				if (Fix(old_val, new_val)) {
					Json::StyledStreamWriter writer;
					std::locale::global(std::locale(""));
					std::ofstream fout(new_filepath.c_str());
					std::locale::global(std::locale("C"));	
					writer.write(fout, new_val);
					fout.close();
				}

				break;
			}
		}
	}
}

bool FixLRMirror::Fix(const Json::Value& old_val, Json::Value& new_val) const
{
	bool dirty = false;
	for (int i = 0, n = old_val["layer"].size(); i < n; ++i) {
		const Json::Value& layer_val = old_val["layer"][i];
		for (int j = 0, m = layer_val["sprite"].size(); j < m; ++j) {
			if (FixSprite(layer_val["sprite"][j], new_val)) {
				dirty = true;
			}
		}
	}
	return dirty;
}

bool FixLRMirror::FixSprite(const Json::Value& old_val, Json::Value& new_val) const
{
	bool dirty = false;

	std::string filepath = old_val["filepath"].asString();

	if (filepath == ee::SYM_GROUP_TAG) 
	{
		const Json::Value& group_val = old_val[ee::SYM_GROUP_TAG];
		for (int i = 0, n = group_val.size(); i < n; ++i) {
			if (FixSprite(group_val[i], new_val)) {
				dirty = true;
			}
		}
		return dirty;
	}

	if (!old_val.isMember("x mirror") && !old_val.isMember("y mirror")) {
		return false;
	}

	for (int i = 0, n = new_val["layer"].size(); i < n; ++i) {
		Json::Value& layer_val = new_val["layer"][i];
		for (int j = 0, m = layer_val["sprite"].size(); j < m; ++j) {
			if (StoreToNew(old_val, layer_val["sprite"][j])) {
				dirty = true;
			}
		}
	}

	return dirty;
}

bool FixLRMirror::StoreToNew(const Json::Value& old_val, Json::Value& new_val) const
{
	std::string name = old_val["name"].asString();

	std::string filepath = new_val["filepath"].asString();
	if (filepath == ee::SYM_GROUP_TAG) 
	{
		bool dirty = false;
		Json::Value& group_val = new_val[ee::SYM_GROUP_TAG];
		for (int i = 0, n = group_val.size(); i < n; ++i) {
			if (group_val[i]["name"].asString() != name) {
				continue;
			}
			if (StoreToNew(old_val, group_val[i])) {
				dirty = true;
			}
		}
		return dirty;
	}

	if (new_val["name"].asString() != name) {
		return false;
	}

	std::string filename = ee::FileHelper::GetFilename(filepath);
	if (old_val["filepath"].asString().find(filename) == std::string::npos) {
		return false;
	}

	sm::vec2 fixed;
	if (FixMirror::Fix(old_val, fixed)) {
		new_val["x scale"] = fixed.x;
		new_val["y scale"] = fixed.y;
		return true;
	} else {
		return false;
	}
}

}