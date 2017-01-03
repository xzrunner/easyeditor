#include "Snapshoot.h"
#include "check_params.h"
#include "utility.h"

#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/Symbol.h>
#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>
#include <sprite2/DrawRT.h>

#include <fstream>
#include <wx/arrstr.h>

namespace edb
{

std::string Snapshoot::Command() const
{
	return "snapshoot";
}

std::string Snapshoot::Description() const
{
	return "get snapshoot from json files";
}

std::string Snapshoot::Usage() const
{
	// snapshoot E:\test2\image E:\test2\image

	return Command() + " [src path] [dst path]";
}

int Snapshoot::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return -1;
	if (!check_folder(argv[2])) return -1;
	if (!check_folder(argv[3])) return -1;

	int ret = init_gl();
	if (ret < 0) {
		return ret;
	}

	Run(argv[2], argv[3]);

	return 0;
}

void Snapshoot::Run(const std::string& srcdir, const std::string& dstdir) const
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(srcdir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		int type = ee::SymbolFile::Instance()->Type(filepath);
		if (type == s2::SYM_COMPLEX || type == s2::SYM_ANIMATION)
		{
			Json::Value value;
			Json::Reader reader;
			std::locale::global(std::locale(""));
			std::ifstream fin(filepath.c_str());
			std::locale::global(std::locale("C"));
			reader.parse(fin, value);
			fin.close();

			std::string name = value["name"].asString();
			if (name.empty()) {
				continue;
			}

			s2::DrawRT rt;
			ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
			rt.Draw(sym);

			std::string filename = dstdir + "//" + name + ".png";
			sm::vec2 sz = sym->GetBounding().Size();
			rt.StoreToFile(filename, sz.x, sz.y);

			sym->RemoveReference();
		}
	}
}

}