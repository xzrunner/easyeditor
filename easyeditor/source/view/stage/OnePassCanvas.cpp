#include "OnePassCanvas.h"

#include <unirender/RenderContext.h>
#include <gum/RenderContext.h>

namespace ee
{

OnePassCanvas::OnePassCanvas(wxWindow* stage_wnd, EditPanelImpl* stage,
							 wxGLContext* glctx)
	: StageCanvas(stage_wnd, stage, glctx)
{
}

void OnePassCanvas::OnDrawWhole() const
{
	ur::RenderContext* rc = gum::RenderContext::Instance()->GetImpl();
	rc->SetClearFlag(ur::MASKC);
	rc->Clear(0);
 
 	OnDrawSprites();
}

}