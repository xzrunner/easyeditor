#ifndef _SIMP_NODE_FACTORY_H_
#define _SIMP_NODE_FACTORY_H_

#include <CU_Uncopyable.h>
#include <CU_Singleton.h>

#include <string>
#include <vector>

#include <stdint.h>

namespace simp
{

class Package;

class NodeFactory : private cu::Uncopyable
{
public:
	void AddPkg(Package* pkg, const std::string& name, int pkg_id);
	const Package* GetPkg(int id) const;

	const void* Create(uint32_t id, int* type);

	uint32_t GetID(const std::string& pkg_name, const std::string& node_name) const;

private:
	static const int PKG_ID_SIZE		= 12;
	static const int NODE_ID_SIZE		= 20;
	static const uint32_t PKG_ID_MASK	= 0xfff00000;
	static const uint32_t NODE_ID_MASK	= 0x000fffff;

	uint32_t GetPkgID(uint32_t id) const {
		return (id & PKG_ID_MASK) >> NODE_ID_SIZE;
	}
	uint32_t GetNodeID(uint32_t id) const {
		return id & NODE_ID_MASK;
	}

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

#include "NodeFactory.inl"

#endif // _SIMP_NODE_FACTORY_H_
