#ifndef _DRAG2D_INODE_CAPTURE_H_
#define _DRAG2D_INODE_CAPTURE_H_

namespace libshape
{

class INodeCapture
{
public:
	virtual ~INodeCapture() {}
	virtual float GetScope() const = 0;
}; // INodeCapture

}

#endif // _DRAG2D_INODE_CAPTURE_H_