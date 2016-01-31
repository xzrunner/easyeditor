#ifndef _EASYPARTICLE3D_EDIT_OP_H_
#define _EASYPARTICLE3D_EDIT_OP_H_



namespace eparticle3d
{

class StagePanel;

class EditOP : public ee::ZoomViewOP
{
public:
	EditOP(StagePanel* stage);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

private:
	StagePanel* m_stage;

}; // EditOP

}

#endif // _EASYPARTICLE3D_EDIT_OP_H_