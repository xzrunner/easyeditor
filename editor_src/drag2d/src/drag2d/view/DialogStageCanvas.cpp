#include "DialogStageCanvas.h"

#include "dataset/ISymbol.h"
#include "view/EditPanel.h"

namespace d2d
{

DialogStageCanvas::DialogStageCanvas(EditPanel* editPanel, 
									 ISymbol* symbol)
	: OrthoCanvas(editPanel)
	, m_symbol(symbol)
{
}

void DialogStageCanvas::initGL()
{
	d2d::OrthoCanvas::initGL();
	m_symbol->reloadTexture();
}

void DialogStageCanvas::onDraw()
{
	m_editPanel->drawEditTemp();
}

} // d2d