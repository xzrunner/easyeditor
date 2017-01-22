#include "ResPacker.h"
#include "PackAnimation.h"
#include "PackNodeFactory.h"
#include "PackUI.h"
#include "PackUIWindowTask.h"
#include "PackTag.h"

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
#include <ee/SymbolType.h>

#include <easycomplex.h>
#include <easyanim.h>
#include <easyparticle3d.h>
#include <easyui.h>
#include <easytrail.h>
#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>

#include <fstream>

namespace erespacker
{

static const int DTEX_PNG4 = 0;
static const int DTEX_PNG8 = 1;
static const int DTEX_PVR  = 2;
static const int DTEX_ETC1 = 3;
static const int DTEX_ETC2 = 4;

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
	for_each(m_syms.begin(), m_syms.end(), cu::RemoveRefFunctor<const ee::Symbol>());
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

void ResPacker::OutputTagKeyVal(const std::string& outfile) const
{
	std::string dir = ee::FileHelper::GetFileDir(outfile);

	Json::Value value;

	PackTag::Instance()->Output(dir, value);

	if (value.isNull()) {
		return;
	}

	std::string filepath = outfile + "_tag.json";
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
		int type = ee::SymbolFile::Instance()->Type(filepath);
		switch (type)
		{
		case s2::SYM_COMPLEX: case s2::SYM_ANIMATION: case s2::SYM_PARTICLE3D: case s2::SYM_TRAIL:
			filepaths.push_back(filepath);
			break;
		case ee::SYM_UI:
			{
				std::string proxy = PackUI::Instance()->AddTask(filepath);
				proxy = ee::FileHelper::GetAbsolutePathFromFile(filepath, proxy);
				ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(proxy);
				m_syms.push_back(sym);
			}
			break;
		case ee::SYM_UIWND:
			PackUI::Instance()->AddWindowTask(filepath);
			AddUIWndSymbol(filepath);
			break;
		}
	}

	std::sort(filepaths.begin(), filepaths.end());
	for (int i = 0, n = filepaths.size(); i < n; ++i) 
	{
		ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepaths[i]);
		if (!sym->name.empty()) {
			m_syms.push_back(sym);
		} else {
			sym->RemoveReference();
		}
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
	for (int i = 0, n = m_syms.size(); i < n; ++i) 
	{
		const ee::Symbol* sym = m_syms[i];
		if (const ecomplex::Symbol* complex = dynamic_cast<const ecomplex::Symbol*>(sym)) {
			factory->Create(complex);
		} else if (const libanim::Symbol* anim = dynamic_cast<const libanim::Symbol*>(sym)) {
			factory->Create(anim);
		} else if (const eparticle3d::Symbol* p3d = dynamic_cast<const eparticle3d::Symbol*>(sym)) {
			factory->Create(p3d);
		} else if (const etrail::Symbol* trail = dynamic_cast<const etrail::Symbol*>(sym)) {
			factory->Create(trail);
		} else {
			throw ee::Exception("ResPacker::Pack unhandled type %s.", sym->GetFilepath());
		}
	}
}

void ResPacker::AddUIWndSymbol(const std::string& filepath)
{
	Json::Value val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	std::vector<ee::Sprite*> sprs;
	eui::window::FileIO::FetchSprites(filepath, sprs);

	ecomplex::Symbol* sym = new ecomplex::Symbol();	
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		sym->Add(sprs[i]);
	}
	for_each(sprs.begin(), sprs.end(), cu::RemoveRefFunctor<ee::Sprite>());

	std::string wrapper_path = PackUIWindowTask::GetWrapperFilepath(filepath);
	sym->SetFilepath(wrapper_path);
	sym->name = val["name"].asString();

	m_syms.push_back(sym);
}

}
