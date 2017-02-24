#include "PackIDMgr.h"
#include "PackNode.h"
#include "typedef.h"

#include <ee/std_functor.h>
#include <ee/FileHelper.h>
#include <ee/Exception.h>
#include <ee/SymbolType.h>

#include <sprite2/SymType.h>
#include <simp/NodeFactory.h>
#include <gum/FilepathHelper.h>
#include <gum/SymbolFile.h>

#include <json/json.h>

#include <algorithm>
#include <fstream>

namespace esprpacker
{

SINGLETON_DEFINITION(PackIDMgr)

PackIDMgr::PackIDMgr()
	: m_curr_pkg(NULL)
	, m_curr_pkg_id(-1)
{
}

PackIDMgr::~PackIDMgr()
{
	for_each(m_pkgs.begin(), m_pkgs.end(), ee::DeletePointerFunctor<Package>());
}

void PackIDMgr::Init(const std::string& filepath, const std::string& platform)
{
	std::string fix = gum::FilepathHelper::Format(filepath);
	m_dir = ee::FileHelper::GetFileDir(fix);
	m_platform = platform;

	if (!ee::FileHelper::IsFileExist(filepath)) {
		return;
	}

	Json::Value val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	m_pkgs.reserve(val.size());
	for (int i = 0, n = val.size(); i < n; ++i)
	{
		const Json::Value& src = val[i];

		Package* pkg = new Package;
		
		pkg->name = src["name"].asString();

		pkg->path = src["path"].asString();
		pkg->path = ee::FileHelper::GetAbsolutePath(m_dir, pkg->path);
		pkg->path = ee::FileHelper::FormatFilepath(pkg->path);

		pkg->id = src["pkg_id"].asInt();

		pkg->img_cut = false;
		if (src.isMember("img_cut")) {
			pkg->img_cut = true;
			pkg->cut_img = src["img_cut"]["img"].asString();
			pkg->cut_json = src["img_cut"]["json"].asString();
			pkg->cut_ori = src["img_cut"]["ori"].asString();
		}

		std::string spr_id_file = GetSprIDFile(pkg->name);
		if (ee::FileHelper::IsFileExist(spr_id_file)) {
			InitSprsID(spr_id_file, pkg);
		}

		m_pkgs.push_back(pkg);
	}
}

void PackIDMgr::AddCurrPath(const std::string& path)
{
	if (m_curr_pkg_id != -1) {
		return;
	}

	std::string fix = gum::FilepathHelper::Format(path);
	m_curr_paths.push_back(fix);

	if (m_curr_pkg) {
		return;
	}

	for (int i = 0, n = m_pkgs.size(); i < n; ++i) {
		if (m_pkgs[i]->path.find(fix) != std::string::npos) {
			m_curr_pkg = m_pkgs[i];
			break;
		}
	}
}

void PackIDMgr::QueryID(const std::string& filepath, int& pkg_id, int& node_id) const
{
	static int NEXT_NODE_ID = 0;
	if (m_curr_pkg_id != -1)
	{
		pkg_id = m_curr_pkg_id;
		node_id = NEXT_NODE_ID++;
		return;
	}
	else if (IsCurrPkg(filepath)) 
	{
		assert(m_curr_pkg);
		pkg_id = m_curr_pkg->id;
		node_id = NEXT_NODE_ID++;
		return;
	}

	Package* pkg = NULL;
	for (int i = 0, n = m_pkgs.size(); i < n; ++i) {
		if (filepath.find(m_pkgs[i]->path) != std::string::npos) {
			pkg = m_pkgs[i];
			break;
		}
	}

	if (!pkg) {
		throw ee::Exception("query pkg id fail: %s", filepath.c_str());
	}

	pkg_id = pkg->id;
	
	std::map<std::string, int>::iterator itr = pkg->sprs.find(filepath);
	if (itr == pkg->sprs.end()) {
		throw ee::Exception("query spr id fail: %s", filepath.c_str());
	}
	node_id = simp::NodeID::GetNodeID(itr->second);
}

bool PackIDMgr::IsCurrPkg(const std::string& filepath) const
{
	if (m_curr_pkg_id != -1) {
		return true;
	}
	if (filepath.empty() || filepath == ee::SYM_GROUP_TAG || filepath == SPRITE_FILEPATH) {
		return true;
	}
	if (gum::SymbolFile::Instance()->Type(filepath) == s2::SYM_TEXTBOX) {
		return true;
	}
	for (int i = 0, n = m_curr_paths.size(); i < n; ++i) {
		if (filepath.find(m_curr_paths[i]) != std::string::npos) {
			return true;
		}
	}
	return false;
}

bool PackIDMgr::IsCurrPkg(const PackNode* node) const
{
	int curr_pkg_id = (m_curr_pkg_id != -1 ? m_curr_pkg_id : m_curr_pkg->id);
	return curr_pkg_id == node->GetPkgID();
}

std::string PackIDMgr::GetSprIDFile(const std::string& pkg_name) const
{
	return m_dir + "\\spr" + "\\" + m_platform + "\\" + pkg_name + ".json";
}

void PackIDMgr::GetCurrImgCutPath(std::string& img, std::string& json, std::string& ori) const
{
	if (!m_curr_pkg) {
		return;
	}

	img = m_curr_pkg->cut_img;
	json = m_curr_pkg->cut_json;
	ori = m_curr_pkg->cut_ori;
}

void PackIDMgr::InitSprsID(const std::string& filepath, Package* pkg) const
{
	Json::Value val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	for (int i = 0, n = val.size(); i < n; ++i) 
	{
		const Json::Value& spr_val = val[i];
		std::string filepath = spr_val["file"].asString();

		filepath = gum::FilepathHelper::Absolute(pkg->path, filepath);
		if (!gum::FilepathHelper::Exists(filepath)) {
			continue;
		}
		filepath = gum::FilepathHelper::Format(filepath);
		int id = spr_val["id"].asInt();

		if (pkg->sprs.find(filepath) != pkg->sprs.end()) {
			throw ee::Exception("PackIDMgr::InitSprsID: dup filepath %s", filepath.c_str());
		}

		pkg->sprs.insert(std::make_pair(filepath, id));
	}
}

}