#include "FixMirror.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/SymbolFile.h>

#include <SM_Vector.h>
#include <sprite2/Color.h>
#include <sprite2/SymType.h>
#include <gum/trans_color.h>

#include <fstream>

namespace edb
{

std::string FixMirror::Command() const
{
	return "fix-mirror";
}

std::string FixMirror::Description() const
{
	return "fix err mirror";
}

std::string FixMirror::Usage() const
{
	return Command() + " [dir]";
}

int FixMirror::Run(int argc, char *argv[])
{
	// fix-mirror D:\projects\ejoy\coco-tools\sg_particle\data\json_and_pngs

	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	Trigger(argv[2]);

	return 0;
}

bool FixMirror::Fix(const Json::Value& val, sm::vec2& fixed)
{
	bool dirty = false;

	if (val.isMember("scale")) {
		fixed.x = val["scale"].asDouble();
	} else if (val.isMember("x scale")) {
		fixed.x = val["x scale"].asDouble();
	} else {
		fixed.x = 1;
	}
	if (val.isMember("scale")) {
		fixed.y = val["scale"].asDouble();
	} else if (val.isMember("y scale")) {
		fixed.y = val["y scale"].asDouble();
	} else {
		fixed.y = 1;
	}

	if (val.isMember("x mirror")) 
	{
		bool mirror = val["x mirror"].asBool();
		if (mirror && fixed.x >= 0 ||
			!mirror && fixed.x < 0) {
				dirty = true;
				fixed.x = -fixed.x;
		}
	}

	if (val.isMember("y mirror")) 
	{
		bool mirror = val["y mirror"].asBool();
		if (mirror && fixed.y >= 0 ||
			!mirror && fixed.y < 0) {
				dirty = true;
				fixed.y = -fixed.y;
		}
	}

	return dirty;
}

bool FixMirror::FixSprite(const std::string& filepath, Json::Value& val) const
{
	sm::vec2 fixed;
	if (Fix(val, fixed)) {
 		val["x scale"] = fixed.x;
 		val["y scale"] = fixed.y;
		return true;
	} else {
		return false;
	}
}

void FixMirror::Trigger(const std::string& dir) const
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		int type = ee::SymbolFile::Instance()->Type(filepath);
		switch (type)
		{
		case s2::SYM_COMPLEX:
			FixComplex(filepath);
			break;
		case s2::SYM_ANIMATION:
			FixAnim(filepath);
			break;
		case s2::SYM_SCALE9:
			FixScale9(filepath);
			break;
		case s2::SYM_UNKNOWN:
			if (filepath.find("_lr.json") != std::string::npos) {
				FixLR(filepath);
			}
			break;
		}
	}
}

}