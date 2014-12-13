#ifndef _ESHADER_STAGE_CANVAS_H_
#define _ESHADER_STAGE_CANVAS_H_

#include <drag2d.h>

namespace eshader
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(StagePanel* stage);
	virtual ~StageCanvas();

protected:
	virtual void onDraw();

private:
	void DrawBackground() const;
	void DrawSprites() const;

private:
	StagePanel* m_stage;

}; // StageCanvas

}

#endif // _ESHADER_STAGE_CANVAS_H_