#include "OnePassCanvas.h"

#include <unirender/RenderContext.h>
#include <gum/RenderContext.h>

namespace ee
{

OnePassCanvas::OnePassCanvas(wxWindow* stage_wnd, EditPanelImpl* stage,
							 wxGLContext* glctx, bool use_context_stack, bool is_3d)
	: StageCanvas(stage_wnd, stage, glctx, use_context_stack, is_3d)
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