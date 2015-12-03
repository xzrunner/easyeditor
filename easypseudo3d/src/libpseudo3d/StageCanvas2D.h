#ifndef _EASYPSEUDO3D_STAGE_CANVAS_2D_H_
#define _EASYPSEUDO3D_STAGE_CANVAS_2D_H_

#include <drag2d.h>

namespace epseudo3d
{

class IScene;

class StageCanvas2D : public d2d::OrthoCanvas
{
public:
	StageCanvas2D(wxWindow* stage_wnd, d2d::EditPanelImpl* stage);

protected:
	virtual void OnDrawSprites() const;

private:
	IScene* m_scene;

}; // StageCanvas2D

}

#endif // _EASYPSEUDO3D_STAGE_CANVAS_2D_H_