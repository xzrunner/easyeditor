#ifndef _DRAG2D_IOPERATER_BASE_H_
#define _DRAG2D_IOPERATER_BASE_H_

namespace eshape
{

class IOperaterBase
{
public:
	virtual ~IOperaterBase() {}
	virtual bool OnMouseLeftDownBase(int x, int y) = 0;
	virtual bool OnMouseRightDownBase(int x, int y) = 0;

}; // IOperaterBase

}

#endif // _DRAG2D_IOPERATER_BASE_H_