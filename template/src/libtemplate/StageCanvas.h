#ifndef _EASYTEMPLATE_STAGE_CANVAS_H_
#define _EASYTEMPLATE_STAGE_CANVAS_H_

#include <ee/TwoPassCanvas.h>

namespace etemplate
{

class StagePanel;

//class StageCanvas : public ee::OrthoCanvas
class StageCanvas : public ee::TwoPassCanvas
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

#endif // _EASYTEMPLATE_STAGE_CANVAS_H_