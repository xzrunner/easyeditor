#include "PackIDMgr.h"
#include "std_functor.h"
#include "FileHelper.h"
#include "Exception.h"

#include <gum/FilepathHelper.h>

#include <json/json.h>

#include <algorithm>
#include <fstream>

namespace ee
{

SINGLETON_DEFINITION(PackIDMgr)

PackIDMgr::PackIDMgr()
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

		std::string name = pkg->name;
		std::string::size_type pos = pkg->name.find('/');
		if (pos != std::string::npos) {
			name = name.substr(pos + 1);
		}
		std::string spr_id_file = GetSprIDFile(name);
		if (ee::FileHelper::IsFileExist(spr_id_file)) {
			InitSprsID(spr_id_file, pkg);
		}

		m_pkgs.push_back(pkg);
	}
}

const PackIDMgr::Package* PackIDMgr::QueryPkg(const std::string& path, bool shorter) const
{
	if (shorter)
	{
		for (int i = 0, n = m_pkgs.size(); i < n; ++i) {
			if (m_pkgs[i]->path.find(path) != std::string::npos) {
				return m_pkgs[i];
			}
		}
	}
	else
	{
		for (int i = 0, n = m_pkgs.size(); i < n; ++i) {
			if (path.find(m_pkgs[i]->path) != std::string::npos) {
				return m_pkgs[i];
			}
		}
	}
	return NULL;
}

uint32_t PackIDMgr::QueryNode(const std::string& filepath) const
{
	const Package* pkg = QueryPkg(filepath, false);
	if (!pkg) {
		return 0xffffffff;
	}

	std::map<std::string, uint32_t>::const_iterator itr = pkg->sprs.find(filepath);
	if (itr != pkg->sprs.end()) {
		return itr->second;
	} else {
		return 0xffffffff;
	}
}

std::string PackIDMgr::GetSprIDFile(const std::string& pkg_name) const
{
	return m_dir + "\\spr" + "\\" + m_platform + "\\" + pkg_name + ".json";
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
		uint32_t id = spr_val["id"].asUInt();

		if (pkg->sprs.find(filepath) != pkg->sprs.end()) {
			throw ee::Exception("PackIDMgr::InitSprsID: dup filepath %s", filepath.c_str());
		}

		pkg->sprs.insert(std::make_pair(filepath, id));
	}
}

}