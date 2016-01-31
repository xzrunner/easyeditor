#ifndef _EASYEDITOR_ONE_PASS_CANVAS_H_
#define _EASYEDITOR_ONE_PASS_CANVAS_H_

#include "StageCanvas.h"

namespace ee
{

class OnePassCanvas : public StageCanvas
{
public:
	OnePassCanvas(wxWindow* stage_wnd, EditPanelImpl* stage, 
		wxGLContext* glctx = NULL);

protected:
	virtual void OnDrawWhole() const;

}; // OnePassCanvas

}

#endif // _EASYEDITOR_ONE_PASS_CANVAS_H_