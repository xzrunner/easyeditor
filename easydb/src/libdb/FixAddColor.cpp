#include "FixAddColor.h"
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

std::string FixAddColor::Command() const
{
	return "fix-add";
}

std::string FixAddColor::Description() const
{
	return "fix add color";
}

std::string FixAddColor::Usage() const
{
	return Command() + " [dir]";
}

int FixAddColor::Run(int argc, char *argv[])
{
	// fix-add D:\projects\ejoy\coco-tools\sg_particle\data\json_and_pngs

	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	Trigger(argv[2]);

	return 0;
}

bool FixAddColor::FixSprite(const std::string& filepath, Json::Value& val) const
{
	if (!val.isMember("add color")) {
		return false;
	}

	std::string add = val["add color"].asString();
	assert(!add.empty() && add[0] == '0' && add[1] == 'x');
	if (add[2] != '0' || add[3] != '0') {
		add[2] = add[3] = '0';
		val["add color"] = add;
		return true;
	} else {
		return false;
	}
}

void FixAddColor::Trigger(const std::string& dir) const
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		int type = ee::SymbolFile::Instance()->Type(filepath);
		switch (type)
		{
		case s2::SYM_PARTICLE3D:
			FixParticle3d(filepath);
			break;
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

void FixAddColor::FixParticle3d(const std::string& filepath) const
{
	Json::Value val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	bool dirty = false;
	for (int i = 0, n = val["components"].size(); i < n; ++i) {
		if (val["components"][i]["add_col_begin"]["a"].asInt() != 0 ||
			val["components"][i]["add_col_end"]["a"].asInt() != 0) {
			val["components"][i]["add_col_begin"]["a"] = 0;
			val["components"][i]["add_col_end"]["a"] = 0;
			dirty = true;
		}
	}

	if (dirty) {
		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(filepath.c_str());
		std::locale::global(std::locale("C"));	
		writer.write(fout, val);
		fout.close();
	}
}

}