#include "FixOldScale9.h"
#include "check_params.h"

#include <ee/FileHelper.h>

#include <SM_Vector.h>
#include <sprite2/Color.h>
#include <glue/trans_color.h>

#include <fstream>

namespace edb
{

std::string FixOldScale9::Command() const
{
	return "fix-scale9";
}

std::string FixOldScale9::Description() const
{
	return "fix old scale9";
}

std::string FixOldScale9::Usage() const
{
	return Command() + " [dir]";
}

int FixOldScale9::Run(int argc, char *argv[])
{
	// fix-scale9 D:\projects\ejoy\coco-tools\sg_particle\data\json_and_pngs

	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	Trigger(argv[2]);

	return 0;
}

void FixOldScale9::Trigger(const std::string& dir) const
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (ee::FileType::IsType(filepath, ee::FileType::e_scale9)) {
			FixScale9(filepath);
		}
	}
}

void FixOldScale9::FixScale9(const std::string& path) const
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

	int i = 0;
	Json::Value spriteVal = value["sprite"][i++];
	while (!spriteVal.isNull()) {
		Json::Value& val = value["sprite"][i-1];
		if (FixScale9Spr(val)) {
			dirty = true;
		}
		spriteVal = value["sprite"][i++];
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

bool FixOldScale9::FixScale9Spr(Json::Value& val) const
{
	bool dirty = false;

	sm::vec2 fix_s;

	if (!val["x mirror"].isNull()) 
	{
		bool mirror = val["x mirror"].asBool();
		float scale = 1;
		if (!val["scale"].isNull()) {
			scale = val["scale"].asDouble();
		} else if (!val["x scale"].isNull()) {
			scale = val["x scale"].asDouble();
		} else {
			assert(0);
		}
		fix_s.x = scale;
		if (mirror && scale >= 0 ||
			!mirror && scale < 0) {
			dirty = true;
			fix_s.x = -scale;
		}
	}

	if (!val["y mirror"].isNull()) 
	{
		dirty = true;
		bool mirror = val["y mirror"].asBool();
		float scale = 1;
		if (!val["scale"].isNull()) {
			scale = val["scale"].asDouble();
		} else if (!val["y scale"].isNull()) {
			scale = val["y scale"].asDouble();
		} else {
			assert(0);
		}
		fix_s.y = scale;
		if (mirror && scale >= 0 ||
			!mirror && scale < 0) {
			dirty = true;
			fix_s.y = -scale;
		}
	}

	if (dirty) {
		val["x scale"] = fix_s.x;
		val["y scale"] = fix_s.y;
	}

	return dirty;
}

}