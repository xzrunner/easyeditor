#ifndef _EASYEDITOR_ONE_PASS_CANVAS_H_
#define _EASYEDITOR_ONE_PASS_CANVAS_H_

#include "StageCanvas.h"

namespace ee
{

class OnePassCanvas : public StageCanvas
{
public:
	OnePassCanvas(wxWindow* stage_wnd, EditPanelImpl* stage, 
		wxGLContext* glctx = nullptr, bool use_context_stack = true, bool is_3d = true);

protected:
	virtual void OnDrawWhole() const override;

}; // OnePassCanvas

}

#endif // _EASYEDITOR_ONE_PASS_CANVAS_H_