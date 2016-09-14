#include "NodeFactory.h"
#include "Package.h"

#include <assert.h>

namespace simp
{

SINGLETON_DEFINITION(NodeFactory);

NodeFactory::NodeFactory()
{
}

void NodeFactory::AddPkg(Package* pkg, const std::string& name, int pkg_id)
{
	PackageWrap wrap;
	wrap.pkg = pkg;
	wrap.name = name;
	wrap.id = pkg_id;
	m_pkgs.push_back(wrap);
}

const void* NodeFactory::Create(uint32_t id, int* type)
{
	uint32_t pkg_id = GetPkgID(id);
	uint32_t node_id = GetNodeID(id);
	assert(pkg_id >= 0 && pkg_id < m_pkgs.size());
	return m_pkgs[pkg_id].pkg->QueryNode(node_id, type);
}

uint32_t NodeFactory::GetID(const std::string& pkg_name, const std::string& node_name) const
{
	for (int i = 0, n = m_pkgs.size(); i < n; ++i) {
		const PackageWrap& pkg = m_pkgs[i];
		if (pkg.name == pkg_name) {
			uint32_t node_id = pkg.pkg->QueryID(node_name);
			uint32_t id = (i << NODE_ID_SIZE) | node_id;
			return id;
		}
	}
	return 0xffffffff;
}

}