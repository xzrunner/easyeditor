#include "DialogStageCanvas.h"

#include "common/config.h"
#include "dataset/ISymbol.h"
//#include "render/DynamicTexture.h"
#include "render/DynamicTexAndFont.h"
#include "view/EditPanelImpl.h"

namespace d2d
{

DialogStageCanvas::DialogStageCanvas(wxWindow* stage_wnd, EditPanelImpl* stage, ISymbol* symbol)
	: OrthoCanvas(stage_wnd, stage)
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