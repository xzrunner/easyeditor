#ifndef _DRAG2D_DIALOG_STAGE_CANVAS_H_
#define _DRAG2D_DIALOG_STAGE_CANVAS_H_

#include "OrthoCanvas.h"

namespace d2d
{

class Symbol;

class DialogStageCanvas : public OrthoCanvas
{
public:
	DialogStageCanvas(wxWindow* stage_wnd, EditPanelImpl* stage, 
		Symbol* symbol, wxGLContext* glctx);

protected:
	virtual void OnDrawSprites() const;

private:
	Symbol* m_symbol;

}; // DialogStageCanvas

}

#endif // _DRAG2D_DIALOG_STAGE_CANVAS_H_