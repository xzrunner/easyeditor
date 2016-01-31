#ifndef _LIBSHAPE_EDIT_NODES_ATOMIC_OP_H_
#define _LIBSHAPE_EDIT_NODES_ATOMIC_OP_H_



namespace libshape
{

class ChainShape;

namespace edit_nodes
{

class ModifyNodesAOP : public ee::AtomicOP
{
public:
	ModifyNodesAOP(const std::vector<ChainShape*>& chains, 
		const std::vector<std::vector<ee::Vector> >& dstChains);

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<ee::Sprite*>& sprites) const { return NULL; }

private:
	std::vector<ChainShape*> m_chains;
	std::vector<std::vector<ee::Vector> > m_src, m_dst;

}; // ModifyNodesAOP

}

}

#endif // _LIBSHAPE_EDIT_NODES_ATOMIC_OP_H_