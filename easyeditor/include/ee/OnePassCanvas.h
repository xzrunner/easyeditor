#ifndef _EASYEDITOR_ONE_PASS_CANVAS_H_
#define _EASYEDITOR_ONE_PASS_CANVAS_H_

#include "StageCanvas.h"

namespace ee
{

class OnePassCanvas : public StageCanvas
{
public:
	OnePassCanvas(wxWindow* stage_wnd, EditPanelImpl* stage, 
		wxGLContext* glctx = nullptr, uint32_t flag = USE_CONTEXT_STACK | HAS_2D);

protected:
	virtual void OnDrawWhole() const override;

}; // OnePassCanvas

}

#endif // _EASYEDITOR_ONE_PASS_CANVAS_H_