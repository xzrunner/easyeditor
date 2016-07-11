#include "ResPacker.h"
#include "PackAnimation.h"
#include "PackNodeFactory.h"
#include "PackUI.h"

#include "PackToLuaString.h"
#include "PackToBin.h"

#include "TextBuilder.h"

#include <ee/TextureFactory.h>
#include <ee/Config.h>
#include <ee/SettingData.h>
#include <ee/FileHelper.h>
#include <ee/StringHelper.h>
#include <ee/Exception.h>
#include <ee/SymbolMgr.h>

#include <easycomplex.h>
#include <easyanim.h>
#include <easyparticle3d.h>

#include <dtex_typedef.h>

#include <fstream>

namespace erespacker
{

// ResPacker::ResPacker(const std::string& json_dir, const std::string& tp_name, 
// 					 const std::string& tp_dir, const std::string& pkgs_cfg_file)
// 	: m_tp(tp_dir)
// 	, m_pkgs(pkgs_cfg_file)
ResPacker::ResPacker(const std::string& json_dir, const std::string& tp_name, 
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

ResPacker::~ResPacker()
{
	for_each(m_symbols.begin(), m_symbols.end(), ee::ReleaseObjectFunctor<const ee::Symbol>());
	PackNodeFactory::Instance()->Release();
}

void ResPacker::OutputLua(const std::string& outfile, float scale) const
{
	PackToLuaString::Pack(outfile, m_tp, scale);
}

void ResPacker::OutputEpe(const std::string& outfile, bool compress, float scale) const
{
	PackToBin::PackEPE(outfile + ".epe", m_tp, compress, scale);
}

void ResPacker::OutputEpt(const std::string& outfile, int LOD, float scale) const
{
	PackToBin::PackEPT(outfile, m_tp, LOD, scale);
}

void ResPacker::OutputUIExtra(const std::string& outfile) const
{
	std::string dir = ee::FileHelper::GetFileDir(outfile);

	Json::Value value;

	PackUI::Instance()->Output(dir, value);
	PackNodeFactory::Instance()->GetTextBuilder()->OutputExtraInfo(value);

	if (value.isNull()) {
		return;
	}

	std::string filepath = outfile + "_ui.json";
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));
	writer.write(fout, value);
	fout.close();
}

void ResPacker::OutputSprID(const std::string& outfile) const
{
	std::vector<IPackNode*> nodes;
	PackNodeFactory::Instance()->GetAllNodes(nodes);
	if (nodes.empty()) {
		return;
	}

	Json::Value value;
	for (int i = 0, n = nodes.size(); i < n; ++i) {
		IPackNode* node = nodes[i];
		if (node->GetPkgID() != 0xffff) {
			continue;
		}

		Json::Value item;
		item["file"] = node->GetFilepath();
		item["id"] = node->GetSprID();
		value[value.size()] = item;
	}
	
	std::string filepath = outfile + "_spr_id.json";
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));
	writer.write(fout, value);
	fout.close();	
}

void ResPacker::OutputEptDesc(const std::string& outfile) const
{
	std::string filepath = outfile + ".ept";
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));	

	const std::vector<const ee::TexturePacker::Texture*>& textures = m_tp.GetTextures();
	
	int count = textures.size();

	int out_sz = 0;
	out_sz += sizeof(int);						// version
	out_sz += sizeof(int);						// count
	out_sz += count * sizeof(uint16_t) * 2;		// size
	out_sz += count * sizeof(uint16_t);			// type

	// file header
	out_sz = -out_sz;
	fout.write(reinterpret_cast<const char*>(&out_sz), sizeof(out_sz));

	// version
	int version = -1;
	fout.write(reinterpret_cast<const char*>(&version), sizeof(version));

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
		uint16_t type = DTEX_PNG8;
		if (tex->format == "pvr") {
			type = DTEX_PVR;
		} else if (tex->format == "etc1") {
			type = DTEX_ETC1;
		} else if (tex->format == "etc2") {
			type = DTEX_ETC2;
		}
		fout.write(reinterpret_cast<const char*>(&type), sizeof(type));
	}

	fout.close();
}

void ResPacker::LoadJsonData(const std::string& dir)
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
			ee::FileType::IsType(filepath, ee::FileType::e_particle3d)) {
			filepaths.push_back(filepath);
		} else if (ee::FileType::IsType(filepath, ee::FileType::e_ui)) {
			PackUI::Instance()->AddTask(filepath);
		}
	}

	std::sort(filepaths.begin(), filepaths.end());
	for (int i = 0, n = filepaths.size(); i < n; ++i) {
		ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepaths[i]);
		m_symbols.push_back(symbol);
	}
}

void ResPacker::LoadTPData(const std::string& tp_name)
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

void ResPacker::Pack() const
{
	PackNodeFactory* factory = PackNodeFactory::Instance();
	for (int i = 0, n = m_symbols.size(); i < n; ++i) 
	{
		const ee::Symbol* symbol = m_symbols[i];
		if (const ecomplex::Symbol* complex = dynamic_cast<const ecomplex::Symbol*>(symbol)) {
			factory->Create(complex);
		} else if (const eanim::Symbol* anim = dynamic_cast<const eanim::Symbol*>(symbol)) {
			factory->Create(anim);
		} else if (const eparticle3d::Symbol* p3d = dynamic_cast<const eparticle3d::Symbol*>(symbol)) {
			factory->Create(p3d);
		} else {
			throw ee::Exception("ResPacker::Pack unhandled type.");
		}
	}
}

}
