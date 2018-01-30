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

void OnePassCanvas::OnSize(int w, int h)
{
	gum::RenderContext::Instance()->OnSize(w, h);
}

void OnePassCanvas::OnDrawWhole() const
{
	ur::RenderContext* rc = gum::RenderContext::Instance()->GetImpl();
	rc->Clear(0x88888888);
 
 	OnDrawSprites();
}

}