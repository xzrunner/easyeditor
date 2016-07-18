#ifndef _EASYTRAIL_STAGE_CANVAS_H_
#define _EASYTRAIL_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>

namespace etrail
{

class StagePanel;

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(StagePanel* stage);

protected:
	virtual void OnDrawSprites() const;

private:
	void DrawBackground() const;

private:
	StagePanel* m_stage;

}; // StageCanvas

}

#endif // _EASYTRAIL_STAGE_CANVAS_H_