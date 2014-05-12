#ifndef _EASYPARTICLE2D_EDIT_OP_H_
#define _EASYPARTICLE2D_EDIT_OP_H_

#include <drag2d.h>

namespace eparticle2d
{

class StagePanel;

class EditOP : public d2d::ZoomViewOP
{
public:
	EditOP(StagePanel* stage);

	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseDrag(int x, int y);

private:
	StagePanel* m_stage;

}; // EditOP

}

#endif // _EASYPARTICLE2D_EDIT_OP_H_
