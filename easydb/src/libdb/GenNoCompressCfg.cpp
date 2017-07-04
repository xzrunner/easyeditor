#include "GenNoCompressCfg.h"
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

GenNoCompressCfg::GenNoCompressCfg()
{
}

std::string GenNoCompressCfg::Command() const
{
	return "gen-no-compress";
}

std::string GenNoCompressCfg::Description() const
{
	return "gen-no-compress";	
}

std::string GenNoCompressCfg::Usage() const
{
	return Command() + " [src dir] [src cfg] [dst cfg]";
}

int GenNoCompressCfg::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 5)) return -1;
	if (!check_folder(argv[2])) return -1;

	Trigger(argv[2], argv[3], argv[4]);

	return 0;
}

void GenNoCompressCfg::Trigger(const std::string& src_dir, const std::string& src_cfg, 
							   const std::string& dst_dir)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(src_dir, files);
	
	std::set<std::string> paths;

	// read from old cfg
	Json::Value src_value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(src_cfg.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, src_value);
	fin.close();
	if (src_value.isMember("no_compress")) {
		for (int i = 0, n = src_value["no_compress"].size(); i < n; ++i) {
			std::string path = src_value["no_compress"][i].asString();
			std::string full_path = gum::FilepathHelper::Absolute(src_dir, path);
			paths.insert(gum::FilepathHelper::Format(full_path));
		}
	}

	// add scale9
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath();
		int type = ee::SymbolFile::Instance()->Type(filepath);
		if (type != s2::SYM_SCALE9) {
			continue;
		}

		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(filepath.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		int zero = 0;

		std::string dir = ee::FileHelper::GetFileDir(filepath);
		int s9_type = value["type"].asInt();
		switch (s9_type)
		{
		case s2::S9_NULL:
			break;
		case s2::S9_9GRID:
			AddPath(paths, dst_dir, value["sprite"][1], dir);
			AddPath(paths, dst_dir, value["sprite"][3], dir);
			AddPath(paths, dst_dir, value["sprite"][4], dir);
			AddPath(paths, dst_dir, value["sprite"][5], dir);
			AddPath(paths, dst_dir, value["sprite"][7], dir);
			break;
		case s2::S9_3GRID_HORI:
			AddPath(paths, dst_dir, value["sprite"][1], dir);
			break;
		case s2::S9_3GRID_VERT:
			AddPath(paths, dst_dir, value["sprite"][1], dir);
			break;
		case s2::S9_6GRID_UPPER:
			AddPath(paths, dst_dir, value["sprite"][zero], dir);
			AddPath(paths, dst_dir, value["sprite"][1], dir);
			AddPath(paths, dst_dir, value["sprite"][2], dir);
			AddPath(paths, dst_dir, value["sprite"][4], dir);
			break;
		case s2::S9_9GRID_HOLLOW:
			AddPath(paths, dst_dir, value["sprite"][1], dir);
			AddPath(paths, dst_dir, value["sprite"][3], dir);
			AddPath(paths, dst_dir, value["sprite"][4], dir);
			AddPath(paths, dst_dir, value["sprite"][6], dir);
			break;
		}

		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(filepath.c_str());
		std::locale::global(std::locale("C"));	
		writer.write(fout, value);
		fout.close();
	}

	// output
	std::ofstream f_compress((dst_dir + "\\compress.tmp").c_str());
	std::ofstream f_no_compress((dst_dir + "\\no_compress.tmp").c_str());
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath();
		int type = ee::SymbolFile::Instance()->Type(filepath);
		if (type != s2::SYM_IMAGE) {
			continue;
		}

		filepath = gum::FilepathHelper::Format(filepath);
		if (paths.find(filepath) != paths.end()) {
			f_no_compress << filepath << "\n";
		} else {
			f_compress << filepath << "\n";
		}
	}
	f_compress.close();
	f_no_compress.close();
}

void GenNoCompressCfg::AddPath(std::set<std::string>& dst, 
							   const std::string& dst_dir,
							   const Json::Value& src, 
							   const std::string& src_dir)
{
	std::string path = src["filepath"].asString();
	std::string full_path = gum::FilepathHelper::Absolute(src_dir, path);
	dst.insert(gum::FilepathHelper::Format(full_path));
}

}