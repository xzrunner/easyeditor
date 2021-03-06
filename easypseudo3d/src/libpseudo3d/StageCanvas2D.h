#ifndef _EASYPSEUDO3D_STAGE_CANVAS_2D_H_
#define _EASYPSEUDO3D_STAGE_CANVAS_2D_H_

#include <ee/CameraCanvas.h>

namespace epseudo3d
{

class IScene;

class StageCanvas2D : public ee::CameraCanvas
{
public:
	StageCanvas2D(wxWindow* stage_wnd, ee::EditPanelImpl* stage);

protected:
	virtual void OnDrawSprites() const override;

private:
	IScene* m_scene;

}; // StageCanvas2D

}

#endif // _EASYPSEUDO3D_STAGE_CANVAS_2D_H_