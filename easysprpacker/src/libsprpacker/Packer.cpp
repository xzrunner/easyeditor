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
#include <dtex.h>
#include <gimg_typedef.h>

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
	OutputEptDesc(outfile);
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
		if (ee::FileType::IsType(filepath, ee::FILE_COMPLEX) || 
			ee::FileType::IsType(filepath, ee::FILE_ANIM) ||
			ee::FileType::IsType(filepath, ee::FILE_PARTICLE3D) ||
			ee::FileType::IsType(filepath, ee::FILE_TRAIL)) {
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

void Packer::OutputEptDesc(const std::string& outfile) const
{
	std::string filepath = outfile + ".ept";
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));	

	const std::vector<const ee::TexturePacker::Texture*>& textures = m_tp.GetTextures();

	int count = textures.size();

	int out_sz = 0;
	out_sz += sizeof(int);						// count
	out_sz += count * sizeof(uint16_t) * 2;		// size
	out_sz += count * sizeof(uint16_t);			// type

	// file header
	out_sz = -out_sz;
	fout.write(reinterpret_cast<const char*>(&out_sz), sizeof(out_sz));

	// count
	fout.write(reinterpret_cast<const char*>(&count), sizeof(count));

	// size & type
	for (int i = 0; i < count; ++i) 
	{
		const ee::TexturePacker::Texture* tex = textures[i];

		// size
		uint16_t w = tex->width,
			h = tex->height;
		fout.write(reinterpret_cast<const char*>(&w), sizeof(w));
		fout.write(reinterpret_cast<const char*>(&h), sizeof(h));

		// type
		uint16_t type = GIT_PNG;
		if (tex->format == "pvr") {
			type = GIT_PVR;
		} else if (tex->format == "etc1") {
			type = GIT_ETC1;
		} else if (tex->format == "etc2") {
			type = GIT_ETC2;
		}
		fout.write(reinterpret_cast<const char*>(&type), sizeof(type));
	}

	fout.close();
}

}