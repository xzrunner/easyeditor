#ifndef _EASYTEMPLATE_STAGE_CANVAS_H_
#define _EASYTEMPLATE_STAGE_CANVAS_H_

#include <drag2d.h>

namespace etemplate
{

class StagePanel;

//class StageCanvas : public d2d::DynamicStageCanvas
class StageCanvas : public d2d::FBOCanvas
{
public:
	StageCanvas(StagePanel* stage);

protected:
	virtual void OnSize(int w, int h);
	virtual void OnDraw() const;
	virtual void onDraw();

private:
	StagePanel* m_stage;

}; // StageCanvas

}

#endif // _EASYTEMPLATE_STAGE_CANVAS_H_