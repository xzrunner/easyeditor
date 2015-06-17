#ifndef _EASYPSEUDO3D_PROJ_2D_EDIT_OP_H_
#define _EASYPSEUDO3D_PROJ_2D_EDIT_OP_H_

#include <drag2d.h>

namespace epseudo3d
{

class Proj2DEditOP : public d2d::AbstractEditOP
{
public:
	Proj2DEditOP(d2d::EditPanel* stage);
	virtual ~Proj2DEditOP();

	virtual bool OnKeyDown(int keyCode);

	virtual bool onDraw() const;

	void InitPlayer();

private:
	d2d::ISymbol* m_player;

}; // Proj2DEditOP

}

#endif // _EASYPSEUDO3D_PROJ_2D_EDIT_OP_H_