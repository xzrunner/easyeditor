#ifndef _EASYNAVMESH_STAGE_CANVAS_H_
#define _EASYNAVMESH_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>

namespace enav
{

class StagePanel;

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(StagePanel* stage);

protected:
	virtual void OnDrawSprites() const override;

private:
	StagePanel* m_stage;

}; // StageCanvas

}

#endif // _EASYNAVMESH_STAGE_CANVAS_H_