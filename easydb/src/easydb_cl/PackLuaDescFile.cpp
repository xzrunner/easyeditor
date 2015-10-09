#include "PackLuaDescFile.h"
#include "check_params.h"

#include <drag2d.h>
#include <easytexpacker.h>

namespace edb
{

PackLuaDescFile::~PackLuaDescFile()
{
	for_each(m_symbols.begin(), m_symbols.end(), d2d::ReleaseObjectFunctor<d2d::ISymbol>());
}

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

void PackLuaDescFile::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 6)) return;
	if (!check_folder(argv[2])) return;
	if (!check_folder(argv[4])) return;

	Trigger(argv[2], argv[3], argv[4], argv[5]);
}

void PackLuaDescFile::Trigger(const std::string& json_dir, const std::string& tp_json,
							  const std::string& tp_dir, const std::string& out_file)
{
	LoadJsonFiles(json_dir);
	LoadTexPacker(tp_json, tp_dir);
	
	libcoco::epd::CocoPacker packer(m_symbols, m_tex_mgr);
	packer.Parser();
	packer.Output(out_file.c_str());
}

void PackLuaDescFile::LoadJsonFiles(const std::string& dir)
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir, files);

	for (int i = 0, n = files.size(); i < n; ++i) 
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex) || 
			d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim))
		{
			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
			m_symbols.push_back(symbol);
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
		std::string path = tp_json + d2d::StringTools::ToString(i) + ".json";
		if (wxFileName::FileExists(path)) {
			m_tex_mgr.Add(path, i-1);
		} else {
			break;
		}
		++i;
	}
}

}