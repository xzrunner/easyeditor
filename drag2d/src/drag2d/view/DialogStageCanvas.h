#ifndef _DRAG2D_DIALOG_STAGE_CANVAS_H_
#define _DRAG2D_DIALOG_STAGE_CANVAS_H_

#include "OrthoCanvas.h"

namespace d2d
{

class ISymbol;

class DialogStageCanvas : public OrthoCanvas
{
public:
	DialogStageCanvas(wxWindow* stage_wnd, EditPanelImpl* stage, 
		ISymbol* symbol, wxGLContext* glctx);

protected:
	virtual void OnDrawSprites() const;

private:
	ISymbol* m_symbol;

}; // DialogStageCanvas

}

#endif // _DRAG2D_DIALOG_STAGE_CANVAS_H_