#ifndef _EASYSPRPACKER_PACK_ID_MGR_H_
#define _EASYSPRPACKER_PACK_ID_MGR_H_

#include <ee/PackIDMgr.h>

#include <CU_Singleton.h>

#include <string>
#include <vector>

namespace esprpacker
{

class PackNode;

class PackIDMgr
{
public:
	void QueryID(const std::string& filepath, int& pkg_id, int& node_id) const;

	void AddCurrPath(const std::string& path);

	bool IsCurrPkg(const std::string& filepath) const;
	bool IsCurrPkg(const PackNode* node) const;

	bool IsCurrImgCut() const { return m_curr_pkg ? m_curr_pkg->img_cut : false; }
	void GetCurrImgCutPath(std::string& img, std::string& json, std::string& ori) const;

	void SetCurrPkgID(int id) { m_curr_pkg_id = id; }

private:
	const ee::PackIDMgr::Package* m_curr_pkg;
	int m_curr_pkg_id;

	std::vector<std::string> m_curr_paths;

	SINGLETON_DECLARATION(PackIDMgr)
	
}; // PackIDMgr

}

#endif // _EASYSPRPACKER_PACK_ID_MGR_H_