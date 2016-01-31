#include "PackPkgMgr.h"

namespace librespacker
{

PackPkgMgr::PackPkgMgr(const std::string& filepath)
{
	
}

PackPkgMgr::~PackPkgMgr()
{
	for_each(m_pkgs.begin(), m_pkgs.end(), DeletePointerFunctor<Package>());
}

void PackPkgMgr::LoadPackage(const Json::Value& val, const std::string& dir)
{
	Package* pkg = new Package;

	pkg->name = val["name"].asString();

	pkg->path = val["path"].asString();
	pkg->path = ee::FileHelper::GetAbsolutePath(dir, pkg->path);
	ee::FileHelper::FormatFilepath(pkg->path);

	pkg->id = val["pkg_id"].asInt();

	std::string ids_file = val["spr_id_file"].asString();
	ids_file = ee::FileHelper::GetAbsolutePath(dir, ids_file);
	
	Json::Value ids_val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(ids_file.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, ids_val);
	fin.close();

	for (int i = 0, n = ids_val.size(); i < n; ++i) {
		const Json::Value& spr_val = ids_val[i];
		std::string filepath = spr_val["file"].asString();
		filepath = ee::FileHelper::GetAbsolutePath(pkg->path, filepath);
		ee::FileHelper::FormatFilepath(filepath);
		int id = spr_val["id"].asInt();
		pkg->sprites.insert(std::make_pair(filepath, id));
	}

	m_pkgs.push_back(pkg);
}

}