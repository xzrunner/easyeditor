#ifndef _EASYSHAPE_EDIT_NODES_ATOMIC_OP_H_
#define _EASYSHAPE_EDIT_NODES_ATOMIC_OP_H_

#include <ee/AtomicOP.h>

#include <SM_Vector.h>

namespace eshape
{

class EditedPolyShape;

namespace edit_nodes
{

class ModifyNodesAOP : public ee::AtomicOP
{
public:
	ModifyNodesAOP(const std::vector<std::shared_ptr<EditedPolyShape>>& polylines,
		const CU_VEC<CU_VEC<sm::vec2> >& dst_polylines);

	virtual void Undo() override;
	virtual void Redo() override;

	virtual Json::Value Store(const std::vector<ee::SprPtr>& sprs) const override { return NULL; }

private:
	std::vector<std::shared_ptr<EditedPolyShape>> m_polylines;
	CU_VEC<CU_VEC<sm::vec2> > m_src, m_dst;

}; // ModifyNodesAOP

}

}

#endif // _EASYSHAPE_EDIT_NODES_ATOMIC_OP_H_