#ifndef _EASYTEXT_STAGE_CANVAS_H_
#define _EASYTEXT_STAGE_CANVAS_H_

#include <drag2d.h>

namespace etext
{

class StagePanel;

//class StageCanvas : public d2d::OrthoCanvas
class StageCanvas : public d2d::TwoPassCanvas
{
public:
	StageCanvas(StagePanel* stage);

protected:
	virtual void OnSize(int w, int h);
	virtual void OnDrawSprites() const;

private:
	StagePanel* m_stage;

}; // StageCanvas

}

#endif // _EASYTEXT_STAGE_CANVAS_H_