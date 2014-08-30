#include "DialogStageCanvas.h"

#include "dataset/ISymbol.h"
#include "view/EditPanel.h"
//#include "render/DynamicTexture.h"
#include "render/DynamicTexAndFont.h"

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
	OrthoCanvas::initGL();
	m_symbol->reloadTexture();
	//DynamicTexture::Instance()->ReloadTexture();
	DynamicTexAndFont::Instance()->ReloadTexture();
}

void DialogStageCanvas::onDraw()
{
	m_editPanel->drawEditTemp(m_screen);
}

} // d2d