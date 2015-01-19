#ifndef _EASYEJOY2D_PACKAGE_MANAGER_H_
#define _EASYEJOY2D_PACKAGE_MANAGER_H_

#include <string>
#include <map>

struct ej_package;

namespace eejoy2d
{

class PackageMgr
{
public:
	static PackageMgr* Instance();

	ej_package* Fetch(const std::string& filepath);
	void Remove(const std::string& filepath);

	void Clear();

private:
	PackageMgr();
	~PackageMgr();

private:
	std::map<std::string, ej_package*> m_pkgs;

	static PackageMgr* m_instance;

}; // PackageMgr

}

#endif // _EASYEJOY2D_PACKAGE_MANAGER_H_