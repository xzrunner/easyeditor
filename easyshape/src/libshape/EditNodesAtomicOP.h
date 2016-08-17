#ifndef _EASYSHAPE_EDIT_NODES_ATOMIC_OP_H_
#define _EASYSHAPE_EDIT_NODES_ATOMIC_OP_H_

#include <ee/AtomicOP.h>

#include <SM_Vector.h>

namespace eshape
{

class PolylineShape;

namespace edit_nodes
{

class ModifyNodesAOP : public ee::AtomicOP
{
public:
	ModifyNodesAOP(const std::vector<PolylineShape*>& polylines, 
		const std::vector<std::vector<sm::vec2> >& dst_polylines);

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<ee::Sprite*>& sprs) const { return NULL; }

private:
	std::vector<PolylineShape*> m_polylines;
	std::vector<std::vector<sm::vec2> > m_src, m_dst;

}; // ModifyNodesAOP

}

}

#endif // _EASYSHAPE_EDIT_NODES_ATOMIC_OP_H_