#include "PackIDMgr.h"
#include "PackNode.h"
#include "typedef.h"

#include <simp/NodeID.h>
#include <sprite2/SymType.h>
#include <gum/FilepathHelper.h>
#include <gum/SymbolFile.h>

#include <ee/SymbolType.h>
#include <ee/ImageData.h>
#include <ee/Exception.h>

#include <assert.h>

namespace esprpacker
{

SINGLETON_DEFINITION(PackIDMgr)

PackIDMgr::PackIDMgr()
	: m_curr_pkg(NULL)
	, m_curr_pkg_id(-1)
{
}

void PackIDMgr::AddCurrPath(const std::string& path)
{
	if (m_curr_pkg_id != -1) {
		return;
	}

	std::string fix = gum::FilepathHelper::Format(path);
	m_curr_paths.push_back(fix);
	if (!m_curr_pkg) {
		m_curr_pkg = ee::PackIDMgr::Instance()->QueryPkg(fix, true);
	}
}

bool PackIDMgr::IsCurrPkg(const std::string& filepath) const
{
	if (m_curr_pkg_id != -1) {
		return true;
	}
	if (filepath.empty() || filepath == ee::SYM_GROUP_TAG || filepath == SPRITE_FILEPATH) {
		return true;
	}
	if (gum::SymbolFile::Instance()->Type(filepath) == s2::SYM_TEXTBOX) {
		return true;
	}
	for (int i = 0, n = m_curr_paths.size(); i < n; ++i) {
		if (filepath.find(m_curr_paths[i]) != std::string::npos) {
			return true;
		}
	}
	return false;
}

bool PackIDMgr::IsCurrPkg(const PackNode* node) const
{
	int curr_pkg_id = (m_curr_pkg_id != -1 ? m_curr_pkg_id : m_curr_pkg->id);
	return curr_pkg_id == node->GetPkgID();
}

void PackIDMgr::QueryID(const std::string& filepath, int& pkg_id, int& node_id) const
{
	static int NEXT_NODE_ID = 0;
	if (m_curr_pkg_id != -1)
	{
		pkg_id = m_curr_pkg_id;
		node_id = NEXT_NODE_ID++;
		return;
	}
	else if (IsCurrPkg(filepath)) 
	{
		assert(m_curr_pkg);
		pkg_id = m_curr_pkg->id;
		node_id = NEXT_NODE_ID++;
		return;
	}

	const ee::PackIDMgr::Package* pkg = ee::PackIDMgr::Instance()->QueryPkg(filepath, false);
	if (!pkg) 
	{
		const std::string default_sym = ee::ImageDataMgr::Instance()->GetDefaultSym();
		if (default_sym.empty()) {
			throw ee::Exception("query pkg id fail: %s", filepath.c_str());
		}
		return QueryID(default_sym, pkg_id, node_id);
	}

	pkg_id = pkg->id;

	std::map<std::string, uint32_t>::const_iterator itr = pkg->sprs.find(filepath);
	if (itr == pkg->sprs.end()) {
		const std::string default_sym = ee::ImageDataMgr::Instance()->GetDefaultSym();
		if (default_sym.empty()) {
			throw ee::Exception("query spr id fail: %s", filepath.c_str());
		}
		return QueryID(default_sym, pkg_id, node_id);
	}
	node_id = simp::NodeID::GetNodeID(itr->second);
}

void PackIDMgr::GetCurrImgCutPath(std::string& img, std::string& json, std::string& ori) const
{
	if (!m_curr_pkg) {
		return;
	}

	img = m_curr_pkg->cut_img;
	json = m_curr_pkg->cut_json;
	ori = m_curr_pkg->cut_ori;
}

}