#ifndef _EASYPARTICLE3D_EDIT_OP_H_
#define _EASYPARTICLE3D_EDIT_OP_H_

#include <drag2d.h>

namespace eparticle3d
{

class StagePanel;

class EditOP : public d2d::ZoomViewOP
{
public:
	EditOP(StagePanel* stage);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

private:
	void SetEmitterMat(const d2d::Vector& pos);

private:
	StagePanel* m_stage;

}; // EditOP

}

#endif // _EASYPARTICLE3D_EDIT_OP_H_