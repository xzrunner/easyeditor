#include "Packer.h"
#include "PackNodeFactory.h"
#include "PackToLua.h"
#include "PackToBin.h"

#include <ee/TextureFactory.h>
#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/StringHelper.h>
#include <ee/Symbol.h>

#include <easyrespacker.h>

#include <CU_RefCountObj.h>

#include <wx/arrstr.h>
#include <wx/filename.h>

#include <algorithm>

namespace esprpacker
{

Packer::Packer(const std::string& json_dir, const std::string& tp_name, 
			   const std::string& tp_dir)
	: m_tp(tp_dir)
{
	ee::TextureFactory::Instance()->InitTexturePacker(tp_dir);

	ee::SettingData& data = ee::Config::Instance()->GetSettings();
	bool old_cfg = data.load_image;
	data.load_image = false;

	PackNodeFactory::Instance()->SetFilesDir(json_dir);

	LoadTPData(tp_name);
	LoadJsonData(json_dir);

	Pack();

	data.load_image = old_cfg;
}

Packer::~Packer()
{
	for_each(m_syms.begin(), m_syms.end(), cu::RemoveRefFonctor<const ee::Symbol>());
//	PackNodeFactory::Instance()->Release();
}

void Packer::OutputLua(const std::string& outfile, float scale) const
{
	PackToLua::Pack(outfile, m_tp, scale);
}

void Packer::OutputEpe(const std::string& outfile, bool compress, float scale) const
{
	PackToBin::Pack(outfile, m_tp, compress, scale);
}

void Packer::OutputEpt(const std::string& outfile, int LOD, float scale) const
{
	erespacker::PackToBin::PackEPT(outfile, m_tp, LOD, scale);
}

void Packer::LoadJsonData(const std::string& dir)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);

	std::vector<std::string> filepaths;
	for (int i = 0, n = files.size(); i < n; ++i) 
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath();
		if (ee::FileType::IsType(filepath, ee::FileType::e_complex) || 
			ee::FileType::IsType(filepath, ee::FileType::e_anim) ||
			ee::FileType::IsType(filepath, ee::FileType::e_particle3d) ||
			ee::FileType::IsType(filepath, ee::FileType::e_trail)) {
			filepaths.push_back(filepath);
		}
	}

	std::sort(filepaths.begin(), filepaths.end());
	for (int i = 0, n = filepaths.size(); i < n; ++i) {
		ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepaths[i]);
		m_syms.push_back(sym);
	}
}

void Packer::LoadTPData(const std::string& tp_name)
{
	int i = 1;
	while (true) {
		std::string tp_path = tp_name + ee::StringHelper::ToString(i) + ".json";
		if (ee::FileHelper::IsFileExist(tp_path)) {
			m_tp.Add(tp_path);
			ee::TextureFactory::Instance()->AddTextureFromConfig(tp_path);
		} else {
			break;
		}
		++i;
	}
}

void Packer::Pack() const
{
	PackNodeFactory* factory = PackNodeFactory::Instance();
	for (int i = 0, n = m_syms.size(); i < n; ++i) {
		factory->Create(m_syms[i]);
	}
}

}