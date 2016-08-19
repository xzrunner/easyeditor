#include "CameraCanvas.h"
#include "Camera.h"
#include "RenderContextStack.h"
#include "CameraMgr.h"

#include <shaderlab.h>
#include <sprite2/RenderCtxStack.h>

namespace ee
{

CameraCanvas::CameraCanvas(wxWindow* stage_wnd, EditPanelImpl* stage,
						 wxGLContext* glctx, bool use_context_stack)
	: TwoPassCanvas(stage_wnd, stage, glctx, use_context_stack)
{
}

void CameraCanvas::OnSize(int w, int h)
{
	TwoPassCanvas::OnSize(w, h);

	CameraMgr::Instance()->GetCamera()->UpdateModelView();

	RenderContextStack::Instance()->SetProjection(w, h);

	s2::RenderCtxStack* s2_stack = s2::RenderCtxStack::Instance();
	s2_stack->Pop();
	s2_stack->Push(s2::RenderCtx(w, h));
}

sm::rect CameraCanvas::GetVisibleRegion() const
{
	const s2::RenderCtx* ctx = s2::RenderCtxStack::Instance()->Top();
	if (!ctx) {
		return sm::rect();
	}

	Camera* cam = CameraMgr::Instance()->GetCamera();
	float s = cam->GetScale();
	sm::rect r(sm::vec2(0, 0), ctx->proj_width * s, ctx->proj_height * s);
	r.Translate(cam->GetPosition());
	return r;
}

}