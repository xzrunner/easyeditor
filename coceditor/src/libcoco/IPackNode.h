#ifndef _LIBCOCO_IPACK_NODE_H_
#define _LIBCOCO_IPACK_NODE_H_

namespace libcoco
{

class IPackNode
{
public:
	IPackNode() {
		static int ID = 0;
		m_id = ID++;
	}
	virtual ~IPackNode() {}

private:
	int m_id;

}; // IPackNode

}

#endif // _LIBCOCO_IPACK_NODE_H_