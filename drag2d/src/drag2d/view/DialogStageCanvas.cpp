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

void DialogStageCanvas::InitGL()
{
	OrthoCanvas::InitGL();

	m_symbol->ReloadTexture();
	if (Config::Instance()->IsUseDTex()) {
		DynamicTexAndFont::Instance()->ReloadTexture();
	}
}

void DialogStageCanvas::OnDrawSprites() const
{
	m_stage->DrawEditOP();
}

} // d2d