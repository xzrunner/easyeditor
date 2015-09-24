#ifndef _EASYEJOY2D_PACKAGE_MANAGER_H_
#define _EASYEJOY2D_PACKAGE_MANAGER_H_

#include <string>
#include <map>

struct dtex_package;

namespace eejoy2d
{

class PackageMgr
{
public:
	static PackageMgr* Instance();

	dtex_package* Fetch(const std::string& filepath);
	void Remove(const std::string& filepath);

	void Clear();

private:
	PackageMgr();
	~PackageMgr();

	dtex_package* LoadPackage(const std::string& pkg);

private:
	std::map<std::string, dtex_package*> m_pkgs;

	static PackageMgr* m_instance;

}; // PackageMgr

}

#endif // _EASYEJOY2D_PACKAGE_MANAGER_H_