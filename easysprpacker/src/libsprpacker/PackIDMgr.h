#ifndef _EASYSPRPACKER_PACK_ID_MGR_H_
#define _EASYSPRPACKER_PACK_ID_MGR_H_

#include <CU_Singleton.h>

#include <string>
#include <map>
#include <vector>

namespace esprpacker
{

class PackIDMgr
{
public:
	void Init(const std::string& filepath, const std::string& platform);

	void AddCurrPath(const std::string& path);

	void QueryID(const std::string& filepath, int& pkg_id, int& node_id) const;

	bool IsCurrPkg(const std::string& filepath) const;

	std::string GetSprIDFile(const std::string& pkg_name) const;

private:
	struct Package
	{
		std::string name;
		std::string path;
		int id;

		std::map<std::string, int> sprs;
	};

	void InitSprsID(const std::string& filepath, Package* pkg) const;

private:
	std::string m_dir;
	std::string m_platform;

	std::vector<std::string> m_curr_paths;

	std::vector<Package*> m_pkgs;

	Package* m_curr_pkg;

	SINGLETON_DECLARATION(PackIDMgr)

}; // PackIDMgr

}

#endif // _EASYSPRPACKER_PACK_ID_MGR_H_