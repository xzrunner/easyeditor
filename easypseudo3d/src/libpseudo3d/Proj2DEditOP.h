#ifndef _EASYPSEUDO3D_PROJ_2D_EDIT_OP_H_
#define _EASYPSEUDO3D_PROJ_2D_EDIT_OP_H_

#include <ee/EditOP.h>

namespace ee { class Symbol; }

namespace epseudo3d
{

class Proj2DEditOP : public ee::EditOP
{
public:
	Proj2DEditOP(wxWindow* wnd, ee::EditPanelImpl* stage);
	virtual ~Proj2DEditOP();

	virtual bool OnKeyDown(int keyCode);

	virtual bool onDraw() const;

	void InitPlayer();

private:
	ee::Symbol* m_player;

}; // Proj2DEditOP

}

#endif // _EASYPSEUDO3D_PROJ_2D_EDIT_OP_H_