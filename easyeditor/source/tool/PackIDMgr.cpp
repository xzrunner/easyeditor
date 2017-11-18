#include "PackIDMgr.h"
#include "std_functor.h"
#include "FileHelper.h"
#include "Exception.h"

#include <gum/FilepathHelper.h>
#include <gum/StringHelper.h>

#include <json/json.h>

#include <algorithm>
#include <fstream>

namespace ee
{

CU_SINGLETON_DEFINITION(PackIDMgr)

PackIDMgr::PackIDMgr()
{
}

PackIDMgr::~PackIDMgr()
{
	for_each(m_pkgs.begin(), m_pkgs.end(), ee::DeletePointerFunctor<Package>());
}

void PackIDMgr::Init(const std::string& filepath, const std::string& platform)
{
	CU_STR fix = gum::FilepathHelper::Format(filepath.c_str());
	m_dir = ee::FileHelper::GetFileDir(fix.c_str());
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
		
		pkg->name = src["name"].asString().c_str();

		pkg->path = src["path"].asString().c_str();
		pkg->path = ee::FileHelper::GetAbsolutePath(m_dir, pkg->path);
		pkg->path = ee::FileHelper::FormatFilepath(pkg->path);

		pkg->id = src["pkg_id"].asInt();

		pkg->img_cut = false;
		if (src.isMember("img_cut")) {
			pkg->img_cut = true;
			pkg->cut_img = src["img_cut"]["img"].asString().c_str();
			pkg->cut_json = src["img_cut"]["json"].asString().c_str();
			pkg->cut_ori = src["img_cut"]["ori"].asString().c_str();
		}

		std::string name = pkg->name;
		std::string::size_type pos = pkg->name.find('/');
		if (pos != std::string::npos) {
			name = name.substr(pos + 1);
		}
		InitSprsID(name, pkg);

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

void PackIDMgr::InitSprsID(const std::string& name, Package* pkg) const
{
	std::string filepath = m_dir + "\\spr\\common\\" + name + ".json";
	if (!ee::FileHelper::IsFileExist(filepath)) {
		std::string platform_path = m_dir + "\\spr" + "\\" + m_platform + "\\" + name + ".json";
		if (!ee::FileHelper::IsFileExist(platform_path)) {
			return;
		}
	}

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
		std::string filename = spr_val["file"].asString();
		uint32_t id = spr_val["id"].asUInt();

		pkg->sprs_id_set.insert(id);
		pkg->sprs_name2id.push_back(std::make_pair(gum::StringHelper::UTF8ToGBK(filename.c_str()).c_str(), id));

		CU_STR filepath = gum::FilepathHelper::Absolute(pkg->path.c_str(), filename.c_str());
		if (!gum::FilepathHelper::Exists(filepath.c_str())) {
			continue;
		}
		filepath = gum::FilepathHelper::Format(filepath);

		if (pkg->sprs.find(filepath.c_str()) != pkg->sprs.end()) {
			throw ee::Exception("PackIDMgr::InitSprsID: dup filepath %s", filepath.c_str());
		}

		pkg->sprs.insert(std::make_pair(filepath.c_str(), id));
	}
}

}