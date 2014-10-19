#ifndef _LIBSKETCH_ARRANGE_OP_H_
#define _LIBSKETCH_ARRANGE_OP_H_

#include "SelectOP.h"

namespace libsketch
{

class IArrangeState;

class ArrangeOP : public SelectOP
{
public:
	ArrangeOP(StagePanel* stage);
	virtual ~ArrangeOP();

	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseLeftUp(int x, int y);
	virtual bool onMouseRightDown(int x, int y);
	virtual bool onMouseRightUp(int x, int y);
	virtual bool onMouseDrag(int x, int y);

private:
	IArrangeState* m_state;

}; // ArrangeOP

}

#endif // _LIBSKETCH_ARRANGE_OP_H_