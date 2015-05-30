#include "DialogStageCanvas.h"

#include "common/config.h"
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
	if (Config::Instance()->IsUseDTex()) {
		DynamicTexAndFont::Instance()->ReloadTexture();
	}
}

void DialogStageCanvas::onDraw()
{
	m_stage->drawEditTemp();
}

} // d2d