#include "PackageMgr.h"

#include <drag2d.h>
#include <dtex.h>

namespace eejoy2d
{

PackageMgr* PackageMgr::m_instance = NULL;

PackageMgr* PackageMgr::Instance()
{
	if (!m_instance) {
		m_instance = new PackageMgr();
	}
	return m_instance;
}

PackageMgr::PackageMgr()
{
	const char* cfg =
		"{ \n"
		"	\"open_c1\" : false, \n"
		"	\"open_c2\" : false, \n"
		"	\"open_c3\" : true \n"
		"} \n"
		;
	dtexf_create(cfg);
}

PackageMgr::~PackageMgr()
{
	Clear();

	dtexf_release();
}

dtex_package* PackageMgr::Fetch(const std::string& filepath)
{
	std::map<std::string, dtex_package*>::iterator itr = m_pkgs.find(filepath);
	if (itr == m_pkgs.end()) {
		dtex_package* pkg = LoadPackage(filepath);
		m_pkgs.insert(std::make_pair(filepath, pkg));
		return pkg;
	} else {
		return itr->second;
	}
}

void PackageMgr::Remove(const std::string& filepath)
{
	std::string lowerpath = filepath;
	d2d::StringTools::ToLower(lowerpath);
	std::map<std::string, dtex_package*>::iterator itr = m_pkgs.find(lowerpath);
	if (itr != m_pkgs.end()) {
		m_pkgs.erase(itr);
	}
}

void PackageMgr::Clear()
{
	m_pkgs.clear();
}

dtex_package* PackageMgr::LoadPackage(const std::string& pkg_name)
{
	return NULL;
}

}