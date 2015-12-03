#ifndef _EASYUI_WRAPPER_STAGE_CANVAS_H_
#define _EASYUI_WRAPPER_STAGE_CANVAS_H_

#include "drag2d.h"

namespace eui
{
namespace wrapper
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(StagePanel* stage, wxGLContext* glctx);

protected:
	virtual void OnDrawSprites() const;

private:
	void DrawGuideLines() const;

private:
	StagePanel* m_stage;

}; // StageCanvas

}
}

#endif // _EASYUI_WRAPPER_STAGE_CANVAS_H_