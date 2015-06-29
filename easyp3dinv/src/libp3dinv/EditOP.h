#ifndef _EASYP3DINV_EDIT_OP_H_
#define _EASYP3DINV_EDIT_OP_H_

#include <drag2d.h>

namespace ep3dinv
{

class StagePanel;

class EditOP : public d2d::ZoomViewOP
{
public:
	EditOP(StagePanel* stage);

	virtual bool OnMouseLeftDown(int x, int y);

private:
	StagePanel* m_stage;

}; // EditOP

}

#endif // _EASYP3DINV_EDIT_OP_H_