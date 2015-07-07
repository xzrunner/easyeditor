#ifndef _DRAG2D_DIALOG_STAGE_CANVAS_H_
#define _DRAG2D_DIALOG_STAGE_CANVAS_H_

#include "OrthoCanvas.h"

namespace d2d
{

class EditPanel;
class ISymbol;

class DialogStageCanvas : public OrthoCanvas
{
public:
	DialogStageCanvas(EditPanel* editPanel, ISymbol* symbol);

protected:
	virtual void InitGL();
	virtual void OnDrawSprites() const;

private:
	ISymbol* m_symbol;

}; // DialogStageCanvas

}

#endif // _DRAG2D_DIALOG_STAGE_CANVAS_H_