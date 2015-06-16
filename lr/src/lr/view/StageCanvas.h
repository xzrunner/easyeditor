#ifndef _LR_STAGE_CANVAS_H_
#define _LR_STAGE_CANVAS_H_

#include <drag2d.h>

namespace lr
{

class StagePanel;

class StageCanvas : public d2d::DynamicStageCanvas
{
public:
	StageCanvas(StagePanel* statge);

	void BuildGrids(int w, int h);

protected:
	virtual void OnDraw();

private:
	void DrawRegion() const;
	void DrawGrids() const;

private:
	StagePanel* m_statge;

}; // StageCanvas

}

#endif // _LR_STAGE_CANVAS_H_
