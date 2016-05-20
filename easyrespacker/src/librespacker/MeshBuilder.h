#ifndef _EASYRESPACKER_MESH_BUILDER_H_
#define _EASYRESPACKER_MESH_BUILDER_H_

#include "INodeBuilder.h"
#include "PackMesh.h"

namespace emesh { class Sprite; class Symbol; }

namespace erespacker
{

class ExportNameSet;

class MeshBuilder : public INodeBuilder
{
public:
	MeshBuilder() {}
	virtual ~MeshBuilder();

	virtual void Traverse(ee::Visitor& visitor) const;

	const IPackNode* Create(const emesh::Symbol* sym);

private:
	std::vector<IPackNode*> m_nodes;

}; // MeshBuilder

}

#endif // _EASYRESPACKER_MESH_BUILDER_H_