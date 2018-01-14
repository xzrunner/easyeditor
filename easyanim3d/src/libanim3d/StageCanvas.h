#ifndef _EASYANIM3D_STAGE_CANVAS_H_
#define _EASYANIM3D_STAGE_CANVAS_H_

#include <easynode3d/StageCanvas.h>

namespace enode3d { class StagePanel; }

namespace eanim3d
{

class StageCanvas : public enode3d::StageCanvas
{
public:
	StageCanvas(enode3d::StagePanel* stage);

public:
	virtual void DrawBackground() const override;

}; // StageCanvas

}

#endif // _EASYANIM3D_STAGE_CANVAS_H_