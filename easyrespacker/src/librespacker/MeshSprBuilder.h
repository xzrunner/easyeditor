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
	virtual ~MeshSprBuilder();

	virtual void Traverse(ee::Visitor<IPackNode>& visitor) const;

	const IPackNode* Create(const emesh::Sprite* spr);

private:
	std::vector<IPackNode*> m_nodes;

}; // MeshSprBuilder

}

#endif // _EASYRESPACKER_MESH_SPR_BUILDER_H_