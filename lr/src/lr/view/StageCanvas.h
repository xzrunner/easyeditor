#ifndef _LR_STAGE_CANVAS_H_
#define _LR_STAGE_CANVAS_H_

#include <drag2d.h>

namespace lr
{

class StagePanel;

class StageCanvas : public d2d::DynamicStageCanvas
{
public:
	StageCanvas(StagePanel* stage);

	void BuildGrids(int w, int h);

protected:
	virtual void OnDrawSprites() const;

private:
	void DrawRegion() const;
	void DrawGrids() const;

private:
	StagePanel* m_stage;

}; // StageCanvas

}

#endif // _LR_STAGE_CANVAS_H_
