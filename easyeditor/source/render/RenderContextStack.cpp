#include "RenderContextStack.h"
#include "RenderContext.h"
#include "ScreenCache.h"
#include "EE_DTex.h"
#include "StageCanvas.h"

#include <shaderlab.h>

#include <assert.h>

namespace ee
{

RenderContextStack* RenderContextStack::m_instance = NULL;

RenderContextStack* RenderContextStack::Instance()
{
	if (!m_instance) {
		m_instance = new RenderContextStack;
	}
	return m_instance;
}

RenderContextStack::RenderContextStack()
	: m_curr_ctx(NULL)
{
}

void RenderContextStack::Push(StageCanvas* canvas, RenderContext* render)
{
	Context ctx;
	ctx.canvas = canvas;
	ctx.render = render;
	m_stack.push_back(ctx);
	
	canvas->SetCurrentCanvas();
}

void RenderContextStack::Pop()
{
	assert(!m_stack.empty());
	m_stack.pop_back();
	if (m_stack.empty()) {
		return;
	}

	Context ctx = m_stack.back();

	ctx.canvas->SetCurrentCanvas();

	m_curr_ctx->OnBind();

	int width, height;
	ctx.render->GetProjection(width, height);
	DTex::Instance()->OnSize(width, height);
	ScreenCache::Instance()->SetSize(width, height);
}

void RenderContextStack::SetModelView(const sm::vec2& offset, float scale)
{
	if (m_curr_ctx) {
		m_curr_ctx->SetModelView(offset, scale);
	} else {
		sl::SubjectMVP2::Instance()->NotifyModelview(offset.x, offset.y, scale, scale);
		//	sl::SubjectMVP3::Instance()->NotifyModelview(m_p3d_cam->GetModelViewMat());
	}
}

void RenderContextStack::SetProjection(int width, int height)
{
	if (m_curr_ctx) {
		m_curr_ctx->SetProjection(width, height);
	} else {
		sl::SubjectMVP2::Instance()->NotifyProjection(width, height);

// 		Pseudo3DCamera* cam = static_cast<Pseudo3DCamera*>(CameraMgr::Instance()->GetCamera(CameraMgr::PSEUDO3D));
// 		if (cam) {
// 			sl::SubjectMVP3::Instance()->NotifyProjection(cam->GetProjectMat());
// 		}
	}
}

bool RenderContextStack::GetModelView(sm::vec2& offset, float& scale) const
{
	if (!m_curr_ctx) {
		return false;
	} else {
		m_curr_ctx->GetModelView(offset, scale);
		return true;
	}
}

bool RenderContextStack::GetProjection(int& width, int& height) const
{
	if (!m_curr_ctx) {
		return false;
	} else {
		return m_curr_ctx->GetProjection(width, height);
	}
}

void RenderContextStack::SetCurrCtx(RenderContext* ctx) 
{ 
	m_curr_ctx = ctx; 
}

}