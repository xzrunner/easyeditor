#ifndef _EASYTEMPLATE_STAGE_CANVAS_H_
#define _EASYTEMPLATE_STAGE_CANVAS_H_

#include <ee/OrthoCanvas.h>

namespace etemplate
{

class StagePanel;

class StageCanvas : public ee::OrthoCanvas
{
public:
	StageCanvas(StagePanel* stage);

protected:
	virtual void OnDrawSprites() const;

private:
	StagePanel* m_stage;

}; // StageCanvas

}

#endif // _EASYTEMPLATE_STAGE_CANVAS_H_