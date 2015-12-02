#ifndef _DRAG2D_ATOMIC_TYPE_H_
#define _DRAG2D_ATOMIC_TYPE_H_

namespace d2d
{

enum AtomicType
{
	AT_MOVE = 0,
	AT_ROTATE,
	AT_INSERT,
	AT_DELETE,
	AT_SCALE,
	AT_SHEAR,
	AT_MIRROR
};

}

#endif // _DRAG2D_ATOMIC_TYPE_H_