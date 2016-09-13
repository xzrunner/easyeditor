#ifndef _SIMP_NODE_FACTORY_H_
#define _SIMP_NODE_FACTORY_H_

#include <CU_Uncopyable.h>
#include <CU_Singleton.h>

#include <string>
#include <vector>

namespace simp
{

class Package;

class NodeFactory : private cu::Uncopyable
{
public:

	void AddPkg(Package* pkg, const std::string& name, int id);

	const void* Create(const std::string& pkg, const std::string& node, int* type);

private:
	struct PackageWrap
	{
		std::string name;
		int id;
		Package* pkg;		
	};

private:
	std::vector<PackageWrap> m_pkgs;

	SINGLETON_DECLARATION(NodeFactory);

}; // NodeFactory

}

#endif // _SIMP_NODE_FACTORY_H_