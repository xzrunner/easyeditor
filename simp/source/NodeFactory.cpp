#include "NodeFactory.h"
#include "Package.h"

namespace simp
{

SINGLETON_DEFINITION(NodeFactory);

NodeFactory::NodeFactory()
{
}

void NodeFactory::AddPkg(Package* pkg, const std::string& name, int id)
{
	PackageWrap wrap;
	wrap.pkg = pkg;
	wrap.name = name;
	wrap.id = id;
	m_pkgs.push_back(wrap);
}

const void* NodeFactory::Create(const std::string& pkg_name, const std::string& node_name, int* type)
{
	for (int i = 0, n = m_pkgs.size(); i < n; ++i) {
		const PackageWrap& pkg = m_pkgs[i];
		if (pkg.name == pkg_name) {
			uint32_t id = pkg.pkg->QueryID(node_name);
			return pkg.pkg->QueryNode(id, type);
		}
	}
	return NULL;
}

}