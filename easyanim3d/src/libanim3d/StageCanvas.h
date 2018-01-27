#ifndef _EASYANIM3D_STAGE_CANVAS_H_
#define _EASYANIM3D_STAGE_CANVAS_H_

#include <ee3/StageCanvas.h>

namespace ee3 { class StagePanel; }

namespace eanim3d
{

class StageCanvas : public ee3::StageCanvas
{
public:
	StageCanvas(ee3::StagePanel* stage);

public:
	virtual void DrawBackground() const override;

}; // StageCanvas

}

#endif // _EASYANIM3D_STAGE_CANVAS_H_