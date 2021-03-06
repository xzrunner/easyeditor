#include "PackLuaDescFile.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/StringHelper.h>
#include <ee/SymbolFile.h>

#include <easytexpacker.h>

#include <sprite2/SymType.h>

namespace edb
{

std::string PackLuaDescFile::Command() const
{
	return "pack-lua-desc";
}

std::string PackLuaDescFile::Description() const
{
	return "pack lua desc file";
}

std::string PackLuaDescFile::Usage() const
{
	return Command() + " [json dir] [tp json] [tp dir] [output file]";
}

int PackLuaDescFile::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 6)) return -1;
	if (!check_folder(argv[2])) return -1;
	if (!check_folder(argv[4])) return -1;

	Trigger(argv[2], argv[3], argv[4], argv[5]);

	return 0;
}

void PackLuaDescFile::Trigger(const std::string& json_dir, const std::string& tp_json,
							  const std::string& tp_dir, const std::string& out_file)
{
	LoadJsonFiles(json_dir);
	LoadTexPacker(tp_json, tp_dir);
	
	ecoco::epd::CocoPacker packer(m_syms, m_tex_mgr);
	packer.Parser();
	packer.Output(out_file.c_str());
}

void PackLuaDescFile::LoadJsonFiles(const std::string& dir)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);

	for (int i = 0, n = files.size(); i < n; ++i) 
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		int type = ee::SymbolFile::Instance()->Type(filepath);
		if (type == s2::SYM_COMPLEX || type == s2::SYM_ANIMATION)
		{
			auto sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
			m_syms.push_back(sym);
		}
	}
}

void PackLuaDescFile::LoadTexPacker(const std::string& tp_json,
									const std::string& tp_dir)
{
	m_tex_mgr.SetSrcDataDir(tp_dir);

	int i = 1;
	while (true)
	{
		std::string path = tp_json + ee::StringHelper::ToString(i) + ".json";
		if (ee::FileHelper::IsFileExist(path)) {
			m_tex_mgr.Add(path, i-1);
		} else {
			break;
		}
		++i;
	}
}

}