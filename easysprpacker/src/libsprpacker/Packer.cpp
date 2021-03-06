#include "Packer.h"
#include "PackNodeFactory.h"
#include "PackToLua.h"
#include "PackToBin.h"
#include "PackNode.h"
#include "PackIDMgr.h"
#include "typedef.h"
#include "LabelBuilder.h"

#include <ee/TextureFactory.h>
#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/StringHelper.h>
#include <ee/Symbol.h>
#include <ee/SymbolFile.h>
#include <ee/SymbolType.h>
#include <ee/SymbolLoader.h>
#include <ee/ImageData.h>

#include <easycomplex.h>
#include <easyrespacker.h>
#include <easyui.h>

#include <cu/CU_RefCountObj.h>
#include <gimg_typedef.h>
#include <sprite2/SymType.h>
#include <gum/FilepathHelper.h>
#include <gum/StringHelper.h>
#include <timp/TextureFormat.h>
#include <simp/NodeID.h>

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

	PackNodeFactory* cache = PackNodeFactory::Instance();
	cache->Clear();
	cache->SetFilesDir(json_dir);

	LoadTPData(tp_name);
	LoadJsonData(json_dir);

	Pack();

	data.load_image = old_cfg;
}

Packer::Packer(const std::string& json_dir, const std::string& tp_name, 
			   const std::string& tp_dir, const Json::Value& json_data)
	: m_tp(tp_dir)
{
	ee::TextureFactory::Instance()->InitTexturePacker(tp_dir);

	ee::SettingData& data = ee::Config::Instance()->GetSettings();
	bool old_cfg = data.load_image;
	data.load_image = false;

	PackNodeFactory* cache = PackNodeFactory::Instance();
	cache->Clear();
	cache->SetFilesDir(json_dir);

	LoadTPData(tp_name);
	LoadJsonData(json_dir, json_data);

	Pack();

	data.load_image = old_cfg;
}

void Packer::OutputLua(const std::string& outfile) const
{
	PackToLua::Pack(outfile, m_tp);
}

void Packer::OutputEpe(const std::string& outfile, bool compress, float scale) const
{
	PackToBin::Pack(outfile, m_tp, compress, scale);
}

void Packer::OutputEpt(const std::string& outfile, int LOD) const
{
	OutputEptDesc(outfile, LOD);
	erespacker::PackToBin::PackEPT(outfile, m_tp, LOD);
}

void Packer::OutputUIExtra(const std::string& outfile) const
{
	std::string dir = ee::FileHelper::GetFileDir(outfile);

	Json::Value value;

	erespacker::PackUI::Instance()->Output(dir, value);
	LabelBuilder::Instance()->OutputExtraInfo(value);

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

void Packer::OutputSprID(const std::string& pkg_name, const std::string& res_dir) const
{
	std::vector<PackNode*> nodes;
	PackNodeFactory::Instance()->FetchAll(nodes);
	if (nodes.empty()) {
		return;
	}

	std::sort(nodes.begin(), nodes.end(), [](const PackNode* l, const PackNode* r) -> bool
	{
		return l->GetID() < r->GetID();
	});

	PackIDMgr* id_mgr = PackIDMgr::Instance();

	uint32_t default_sym_id = 0xffffffff;
	const std::string default_sym_path = ee::ImageDataMgr::Instance()->GetDefaultSym();
	if (!default_sym_path.empty()) {
		int pkg_id, node_id;
		PackIDMgr::Instance()->QueryID(default_sym_path, pkg_id, node_id, true);
		default_sym_id = simp::NodeID::ComposeID(pkg_id, node_id);
	}

	Json::Value value;
	for (int i = 0, n = nodes.size(); i < n; ++i) 
	{
		PackNode* node = nodes[i];
		if (!id_mgr->IsCurrPkg(node)) {
			continue;
		}
		
		if (default_sym_id != 0xffffffff && default_sym_id == node->GetID() &&
			default_sym_path != node->GetFilepath()) {
			continue;
		}

		std::string filepath = node->GetFilepath();
		if (ee::SymbolLoader::IsNoFilepathSym(filepath) ||
			filepath.empty() || 
			filepath == SPRITE_FILEPATH || 
			filepath == ee::SYM_GROUP_TAG ||
			filepath == ee::SYM_TEXT_TAG ||
			filepath == ee::SYM_SHAPE_TAG) {
			continue;
		}
		filepath = gum::FilepathHelper::Relative(res_dir.c_str(), filepath.c_str()).c_str();
		if (id_mgr->IsCurrImgCut()) 
		{
			std::string img, json, ori;
			id_mgr->GetCurrImgCutPath(img, json, ori);
			if (filepath.compare(0, img.size(), img) == 0) {
				continue;
			} else if (filepath.compare(0, json.size(), json) == 0) {
				filepath = filepath.substr(json.size() + 1);
				ee::StringHelper::ReplaceAll(filepath, "%", "\\");
				ee::StringHelper::ReplaceAll(filepath, "_complex.json", ".png");
			} else if (ori.empty() || filepath.compare(0, ori.size(), ori) == 0) {
				if (!ori.empty()) {
					filepath = filepath.substr(ori.size() + 1);
				}
			} else {
				throw ee::Exception("Packer::OutputSprID: err file %s", filepath.c_str());
			}
		} 

		if (filepath.empty()) {
			throw ee::Exception("Packer::OutputSprID: empty filepath %s", node->GetFilepath().c_str());
		}

		Json::Value item;
		item["file"] = gum::StringHelper::GBKToUTF8(filepath.c_str()).c_str();
		item["id"] = node->GetID();
		value[value.size()] = item;
	}

	std::string filepath = ee::PackIDMgr::Instance()->GetSprIDFile(pkg_name);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));
	writer.write(fout, value);
	fout.close();	
}

void Packer::OutputTagKeyVal(const std::string& outfile) const
{
	std::string dir = ee::FileHelper::GetFileDir(outfile);

	Json::Value value;

	erespacker::PackTag::Instance()->Output(dir, value);

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

void Packer::LoadJsonData(const std::string& dir)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);

	std::sort(files.begin(), files.end());

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
				std::string proxy = erespacker::PackUI::Instance()->AddTask(filepath);
				proxy = ee::FileHelper::GetAbsolutePathFromFile(filepath, proxy);
				auto sym = ee::SymbolMgr::Instance()->FetchSymbol(proxy);
				m_syms.push_back(sym);
			}
			break;
		case ee::SYM_UIWND:
			erespacker::PackUI::Instance()->AddWindowTask(filepath);
			AddUIWndSymbol(filepath);
			break;
		}
	}

	for (int i = 0, n = filepaths.size(); i < n; ++i) 
	{
		auto sym = ee::SymbolMgr::Instance()->FetchSymbol(filepaths[i]);
		if (!sym->name.empty()) {
			m_syms.push_back(sym);
		}
	}
}

void Packer::LoadJsonData(const std::string& dir, const Json::Value& json_data)
{
	std::vector<std::string> filepaths;
	for (int i = 0, n = json_data.size(); i < n; ++i)
	{
		std::string filepath = gum::FilepathHelper::Absolute(dir.c_str(), json_data[i].asString().c_str()).c_str();
		int type = ee::SymbolFile::Instance()->Type(filepath);
		switch (type)
		{
		case s2::SYM_COMPLEX: case s2::SYM_ANIMATION: case s2::SYM_PARTICLE3D: case s2::SYM_TRAIL:
			filepaths.push_back(filepath);
			break;
		case ee::SYM_UI:
			{
				std::string proxy = erespacker::PackUI::Instance()->AddTask(filepath);
				proxy = ee::FileHelper::GetAbsolutePathFromFile(filepath, proxy);
				auto sym = ee::SymbolMgr::Instance()->FetchSymbol(proxy);
				m_syms.push_back(sym);
			}
			break;
		case ee::SYM_UIWND:
			erespacker::PackUI::Instance()->AddWindowTask(filepath);
			AddUIWndSymbol(filepath);
			break;
		}
	}

	std::sort(filepaths.begin(), filepaths.end());
	for (int i = 0, n = filepaths.size(); i < n; ++i) 
	{
		auto sym = ee::SymbolMgr::Instance()->FetchSymbol(filepaths[i]);
		if (!sym->name.empty()) {
			m_syms.push_back(sym);
		}
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
		if (m_syms[i]) {
			factory->Create(m_syms[i]);
		}
	}
}

void Packer::AddUIWndSymbol(const std::string& filepath)
{
	Json::Value val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	std::vector<ee::SprPtr> sprs;
	eui::window::FileIO::FetchSprites(filepath, sprs);

	auto sym = std::make_shared<ecomplex::Symbol>();	
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		sym->Add(sprs[i]);
	}

	std::string wrapper_path = erespacker::PackUIWindowTask::GetWrapperFilepath(filepath);
	sym->SetFilepath(wrapper_path);
	sym->name = val["name"].asString();

	ecomplex::LoadFromJson::CreateActionsFromTag(*sym);

	m_syms.push_back(sym);
}

void Packer::OutputEptDesc(const std::string& outfile, int LOD) const
{
	std::string filepath = outfile + ".ept";
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));	

	const std::vector<const ee::TexturePacker::Texture*>& textures = m_tp.GetTextures();

	uint16_t tex_count = textures.size();

	int out_sz = 0;
	out_sz += sizeof(uint16_t);						// tex count
	out_sz += sizeof(uint16_t);						// lod count
	out_sz += tex_count * sizeof(uint16_t) * 2;		// size
	out_sz += tex_count * sizeof(uint16_t);			// type

	// file header
	out_sz = -out_sz;
	fout.write(reinterpret_cast<const char*>(&out_sz), sizeof(out_sz));

	// tex count
	fout.write(reinterpret_cast<const char*>(&tex_count), sizeof(tex_count));

	// lod count
	uint16_t lod_count = LOD + 1;
	fout.write(reinterpret_cast<const char*>(&lod_count), sizeof(lod_count));

	// size & type
	for (int i = 0; i < tex_count; ++i) 
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
			type = timp::TEXTURE_PVR4;
		} else if (tex->format == "etc1") {
			type = timp::TEXTURE_ETC1;
		} else if (tex->format == "etc2") {
			type = timp::TEXTURE_ETC2;
		}
		fout.write(reinterpret_cast<const char*>(&type), sizeof(type));
	}

	fout.close();
}

}