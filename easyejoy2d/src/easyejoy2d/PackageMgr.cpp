#include "PackageMgr.h"

#include <drag2d.h>
#include <dtex_facade.h>

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

ej_package* PackageMgr::Fetch(const std::string& filepath)
{
	std::string epd_path = d2d::FilenameTools::getExistFilepath(filepath + ".epd");
	epd_path = d2d::FilenameTools::FormatFilepathAbsolute(epd_path);

	std::string epp_path = d2d::FilenameTools::getExistFilepath(filepath + ".epp");
	epp_path = d2d::FilenameTools::FormatFilepathAbsolute(epp_path);

	std::map<std::string, ej_package*>::iterator itr = m_pkgs.find(epd_path);
	if (itr == m_pkgs.end()) {
		std::string name = d2d::FilenameTools::getFilename(filepath);
		ej_package* pkg = dtexf_c3_load_pkg(name.c_str(), epd_path.c_str(), 1);
		m_pkgs.insert(std::make_pair(epd_path, pkg));
		dtexf_c3_load_pkg(name.c_str(), epp_path.c_str(), 1);
		dtexf_c3_load_pkg_finish();
		return pkg;
	} else {
		return itr->second;
	}
}

void PackageMgr::Remove(const std::string& filepath)
{
	std::string lowerpath = filepath;
	d2d::StringTools::ToLower(lowerpath);
	std::map<std::string, ej_package*>::iterator itr = m_pkgs.find(lowerpath);
	if (itr != m_pkgs.end()) {
		m_pkgs.erase(itr);
	}
}

void PackageMgr::Clear()
{
	m_pkgs.clear();
}

}