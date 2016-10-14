#ifndef _LIBANIM2_STAGE_CANVAS_H_
#define _LIBANIM2_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>

namespace libanim2
{

class StagePanel;

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(StagePanel* stage);

protected:
	virtual void OnDrawSprites() const;

private:
	StagePanel* m_stage;

}; // StageCanvas

}

#endif // _LIBANIM2_STAGE_CANVAS_H_