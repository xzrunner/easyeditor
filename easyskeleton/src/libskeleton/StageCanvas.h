#ifndef _EASYSKELETON_STAGE_CANVAS_H_
#define _EASYSKELETON_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>

namespace eskeleton
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

#endif // _EASYSKELETON_STAGE_CANVAS_H_
