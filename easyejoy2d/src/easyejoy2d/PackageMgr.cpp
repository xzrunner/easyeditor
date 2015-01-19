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
}

PackageMgr::~PackageMgr()
{
	Clear();
}

ej_package* PackageMgr::Fetch(const std::string& filepath)
{
	wxString fixed_path = d2d::FilenameTools::getExistFilepath(filepath);
	wxFileName filename(fixed_path);
	filename.Normalize();
	fixed_path = filename.GetFullPath().Lower();

	std::map<std::string, ej_package*>::iterator itr 
		= m_pkgs.find(fixed_path.ToStdString());
	if (itr == m_pkgs.end()) {
		std::string name = d2d::FilenameTools::getFilename(filepath);
		ej_package* pkg = dtexf_c3_load_pkg("2002jumin", fixed_path, 1);
		m_pkgs.insert(std::make_pair(fixed_path, pkg));
		return pkg;
	} else {
		return itr->second;
	}
}

void PackageMgr::Remove(const std::string& filepath)
{
	wxString lowerpath = filepath;
	lowerpath = lowerpath.Lower();
	std::map<std::string, ej_package*>::iterator itr 
		= m_pkgs.find(lowerpath.ToStdString());
	if (itr != m_pkgs.end()) {
		m_pkgs.erase(itr);
	}
}

void PackageMgr::Clear()
{
	m_pkgs.clear();
}

}