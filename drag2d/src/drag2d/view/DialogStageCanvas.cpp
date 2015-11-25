#include "DialogStageCanvas.h"

#include "common/config.h"
#include "dataset/ISymbol.h"
#include "view/EditPanelImpl.h"
#include "render/DrawCallBatching.h"

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

	if (d2d::Config::Instance()->IsUseDTex()) {
		DrawCallBatching::Instance()->ReloadBegin();
	}
	m_symbol->ReloadTexture();
	if (d2d::Config::Instance()->IsUseDTex()) {
		DrawCallBatching::Instance()->ReloadEnd();
	}
}

void DialogStageCanvas::OnDrawSprites() const
{
	m_stage->DrawEditOP();
}

} // d2d