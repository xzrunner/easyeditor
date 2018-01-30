#include "GenPkgRefInfo.h"
#include "check_params.h"

#include <ee/FileHelper.h>

#include <fs_file.h>
#include <s2loader/PkgFileParser.h>
#include <simp/PkgIdxLoader.h>

#include <wx/arrstr.h>
#include <wx/filename.h>

#include <fstream>

namespace edb
{

GenPkgRefInfo::GenPkgRefInfo()
{

}

std::string GenPkgRefInfo::Command() const
{
	return "pkg-ref";
}

std::string GenPkgRefInfo::Description() const
{
	return "pkg-ref";
}

std::string GenPkgRefInfo::Usage() const
{
	return Command() + " [src dir] [pkg_cfg] [dst path]";
}

int GenPkgRefInfo::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return -1;
	if (!check_folder(argv[2])) return -1;
	if (!check_file(argv[3])) return -1;

	Trigger(argv[2], argv[3], argv[4]);

	return 0;
}

void GenPkgRefInfo::Trigger(const std::string& src_dir, const std::string& pkg_cfg,
							const std::string& dst_filepath)
{
	std::map<int, std::string> pkg_id2name;

	Json::Value val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(pkg_cfg.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	for (int i = 0, n = val.size(); i < n; ++i) 
	{
		std::string name = val[i]["name"].asString();
		int id = val[i]["pkg_id"].asInt();
		pkg_id2name.insert(std::make_pair(id, name));
	}

	std::ofstream fout(dst_filepath.c_str());

	wxArrayString files;
	ee::FileHelper::FetchAllFiles(src_dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath();
		if (filepath.find(".pkg") == std::string::npos) {
			continue;
		}

		fs_file* file = fs_open(filepath.c_str(), "rb");
		assert(file);
		int off = s2loader::PkgFileParser::GetEpeIdxOffset(file);
		CU_MAP<CU_STR, uint32_t> export_names;
		CU_VEC<simp::Package::PageDesc> pages;
		CU_VEC<int> ref_pkgs;
		simp::PkgIdxLoader loader(file, off, export_names, pages, ref_pkgs);
		loader.Load();

		std::string name = ee::FileHelper::GetFilename(filepath);
		fout << name << ": ";

		for (int j = 0, m = ref_pkgs.size(); j < m; ++j)
		{
			std::map<int, std::string>::iterator itr = pkg_id2name.find(ref_pkgs[j]);
			assert(itr != pkg_id2name.end());
			fout << itr->second << " ";
		}
		fout << "\n";
	}

	fout.close();
}

}