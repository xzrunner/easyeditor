#ifndef _EASYEDITOR_PACK_ID_MGR_H_
#define _EASYEDITOR_PACK_ID_MGR_H_

#include <cu/cu_macro.h>

#include <string>
#include <map>
#include <vector>
#include <set>

#include <stdint.h>

namespace ee
{

class PackNode;

class PackIDMgr
{
public:
	struct Package
	{
		std::string name;
		std::string path;
		int id;

		std::map<std::string, uint32_t> sprs;
		std::set<uint32_t> sprs_id_set;
        std::map<std::string, uint32_t> path2id;

		bool img_cut;
		std::string cut_img, cut_json, cut_ori;
	};

public:
	void Init(const std::string& filepath, const std::string& platform);

	const Package* QueryPkg(const std::string& path, bool shorter) const;

	uint32_t QueryNode(const std::string& filepath) const;

	std::string GetSprIDFile(const std::string& pkg_name) const;

private:
	void InitSprsID(const std::string& name, Package* pkg) const;

private:
	std::string m_dir;
	std::string m_platform;

	std::vector<Package*> m_pkgs;

	CU_SINGLETON_DECLARATION(PackIDMgr)

}; // PackIDMgr

}

#endif // _EASYEDITOR_PACK_ID_MGR_H_