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

const void* NodeFactory::Create(const std::string& pkg, const std::string& node, int* type)
{
	for (int i = 0, n = m_pkgs.size(); i < n; ++i) {
		if (m_pkgs[i].name == pkg) {
			return m_pkgs[i].pkg->QueryNode(node, type);
		}
	}
	return NULL;
}

}