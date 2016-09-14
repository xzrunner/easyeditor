#ifndef _SIMP_NODE_FACTORY_INL_
#define _SIMP_NODE_FACTORY_INL_

namespace simp
{

inline 
const Package* NodeFactory::GetPkg(int id) const 
{ 
	uint32_t pkg_id = GetPkgID(id);
	if (pkg_id >= 0 && pkg_id < m_pkgs.size()) {
		return m_pkgs[pkg_id].pkg;
	} else {
		return NULL;
	}
}

}

#endif // _SIMP_NODE_FACTORY_INL_
