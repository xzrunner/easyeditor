#include "FixJsonBase.h"

#include <ee/FileHelper.h>

#include <fstream>

namespace edb
{

void FixJsonBase::FixComplex(const std::string& path) const
{
	std::string filepath = ee::FileHelper::GetAbsolutePath(path);

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	bool dirty = false;

	for (int i = 0, n = value["sprite"].size(); i < n; ++i) {
		if (FixSprite(path, value["sprite"][i])) {
			dirty = true;
		}
	}

	if (dirty) {
		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(filepath.c_str());
		std::locale::global(std::locale("C"));	
		writer.write(fout, value);
		fout.close();
	}
}

void FixJsonBase::FixAnim(const std::string& path) const
{
	std::string filepath = ee::FileHelper::GetAbsolutePath(path);

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	bool dirty = false;

	for (int layer = 0, layer_n = value["layer"].size(); layer < layer_n; ++layer) {
		Json::Value& layer_val = value["layer"][layer];
		for (int frame = 0, frame_n = layer_val["frame"].size(); frame < frame_n; ++frame) {
			Json::Value& frame_val = layer_val["frame"][frame];
			for (int actor = 0, actor_n = frame_val["actor"].size(); actor < actor_n; ++actor) {
				if (FixSprite(path, frame_val["actor"][actor])) {
					dirty = true;
				}
			}
		}
	}

	if (dirty) {
		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(filepath.c_str());
		std::locale::global(std::locale("C"));	
		writer.write(fout, value);
		fout.close();
	}
}

void FixJsonBase::FixScale9(const std::string& path) const
{
	std::string filepath = ee::FileHelper::GetAbsolutePath(path);

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	bool dirty = false;

	for (int i = 0, n = value["sprite"].size(); i < n; ++i) {
		if (FixSprite(path, value["sprite"][i])) {
			dirty = true;
		}
	}

	if (dirty) {
		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(filepath.c_str());
		std::locale::global(std::locale("C"));	
		writer.write(fout, value);
		fout.close();
	}
}

void FixJsonBase::FixLR(const std::string& path) const
{
	std::string filepath = ee::FileHelper::GetAbsolutePath(path);

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	bool dirty = false;

	Json::Value& layer_val = value["layer"];
	for (int i = 0, n = layer_val.size(); i < n; ++i) {
		Json::Value& spr_val = layer_val[i]["sprite"];
		for (int j = 0, m = spr_val.size(); j < m; ++j) {
			if (FixSprite(path, spr_val[j])) {
				dirty = true;
			}
		}
	}

	if (dirty) {
		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(filepath.c_str());
		std::locale::global(std::locale("C"));	
		writer.write(fout, value);
		fout.close();
	}
}

}