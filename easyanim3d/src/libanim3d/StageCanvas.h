#ifndef _EASYANIM3D_STAGE_CANVAS_H_
#define _EASYANIM3D_STAGE_CANVAS_H_

#include <easynode3d/StageCanvas.h>

namespace ee { class MultiSpritesImpl; }

namespace eanim3d
{

class StageCanvas : public enode3d::StageCanvas
{
public:
	StageCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
		ee::MultiSpritesImpl* sprites_impl);

public:
	virtual void DrawBackground() const override;

}; // StageCanvas

}

#endif // _EASYANIM3D_STAGE_CANVAS_H_