#ifndef _EASYRESPACKER_MESH_SPR_BUILDER_H_
#define _EASYRESPACKER_MESH_SPR_BUILDER_H_

#include "INodeBuilder.h"
#include "PackMeshSpr.h"

namespace emesh { class Sprite; }

namespace erespacker
{

class ExportNameSet;

class MeshSprBuilder : public INodeBuilder
{
public:
	MeshSprBuilder(ExportNameSet& export_set);
	virtual ~MeshSprBuilder();

	virtual void Traverse(ee::Visitor& visitor) const;

	const IPackNode* Create(const emesh::Sprite* spr);

private:
	ExportNameSet& m_export_set;

	std::vector<IPackNode*> m_nodes;

}; // MeshSprBuilder

}

#endif // _EASYRESPACKER_MESH_SPR_BUILDER_H_