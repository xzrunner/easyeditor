#include "OnePassCanvas.h"

#include <unirender/RenderContext.h>
#include <gum/RenderContext.h>

namespace ee
{

OnePassCanvas::OnePassCanvas(wxWindow* stage_wnd, EditPanelImpl* stage,
							 wxGLContext* glctx, uint32_t flag)
	: StageCanvas(stage_wnd, stage, glctx, flag)
{
}

void OnePassCanvas::OnDrawWhole() const
{
	ur::RenderContext* rc = gum::RenderContext::Instance()->GetImpl();
	rc->SetClearFlag(ur::MASKC);
	rc->Clear(0x88888888);
 
 	OnDrawSprites();
}

}