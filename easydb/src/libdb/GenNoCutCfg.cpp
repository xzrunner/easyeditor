#include "GenNoCutCfg.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>
#include <sprite2/Scale9.h>
#include <gum/FilepathHelper.h>

#include <wx/arrstr.h>
#include <wx/filename.h>

#include <fstream>

namespace edb
{

GenNoCutCfg::GenNoCutCfg()
{
}

std::string GenNoCutCfg::Command() const
{
	return "gen-no-cut";
}

std::string GenNoCutCfg::Description() const
{
	return "gen-no-cut";	
}

std::string GenNoCutCfg::Usage() const
{
	return Command() + " [src dir] [cfg path]";
}

int GenNoCutCfg::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return -1;
	if (!check_folder(argv[2])) return -1;

	Trigger(argv[2], argv[3]);

	return 0;
}

void GenNoCutCfg::Trigger(const std::string& src_dir, 
	                      const std::string& cfg_path)
{	
	std::set<std::string> no_cut_paths;

	// add mesh for no cut
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(src_dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath();
		if (ee::SymbolFile::Instance()->Type(filepath) != s2::SYM_MESH) {
			continue;
		}

		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(filepath.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		std::string base_sym = value["base_symbol"].asString();
		if (ee::SymbolFile::Instance()->Type(base_sym) != s2::SYM_IMAGE) {
			continue;
		}

		std::string dir = ee::FileHelper::GetFileDir(filepath);
		AddPath(no_cut_paths, base_sym, dir);
	}

	if (no_cut_paths.empty()) {
		return;
	}

	// read from old cfg

	std::set<std::string> no_compress_paths;

	Json::Value src_value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(cfg_path.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, src_value);
	fin.close();
	LoadConfig(src_dir, src_value, "no_compress", no_compress_paths);
	LoadConfig(src_dir, src_value, "no_cut", no_cut_paths);

	// output
	auto cfg_dir = gum::FilepathHelper::Format(
		gum::FilepathHelper::Dir(gum::FilepathHelper::Absolute("", cfg_path.c_str())));
	Json::Value dst_value;
	int idx = 0;
	for (auto& path : no_compress_paths) {
		auto fmt_path = gum::FilepathHelper::Relative(cfg_dir, path.c_str());
		dst_value["no_compress"][idx++] = fmt_path.c_str();
	}
	idx = 0;
	for (auto& path : no_cut_paths) {
		auto fmt_path = gum::FilepathHelper::Relative(cfg_dir, path.c_str());
		dst_value["no_cut"][idx++] = fmt_path.c_str();
	}
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout((cfg_path).c_str());
	std::locale::global(std::locale("C"));
	writer.write(fout, dst_value);
	fout.close();
}

void GenNoCutCfg::LoadConfig(const std::string& src_dir,
	                         const Json::Value& src,
	                         const std::string& key,
	                         std::set<std::string>& dst)
{
	if (src.isMember(key)) {
		return;
	}

	for (int i = 0, n = src[key].size(); i < n; ++i) {
		std::string path = src[key][i].asString();
		auto full_path = gum::FilepathHelper::Absolute(src_dir.c_str(), path.c_str());
		dst.insert(gum::FilepathHelper::Format(full_path.c_str()).c_str());
	}
}

void GenNoCutCfg::AddPath(std::set<std::string>& dst, 
	                      const std::string& src,
						  const std::string& src_dir)
{
	auto full_path = gum::FilepathHelper::Absolute(src_dir.c_str(), src.c_str());
	dst.insert(gum::FilepathHelper::Format(full_path).c_str());
}

}