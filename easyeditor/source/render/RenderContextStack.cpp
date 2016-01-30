#include "RenderContextStack.h"
#include "RenderContext.h"
#include "DrawCallBatching.h"
#include "ScreenCache.h"
#include "ShaderMgr.h"

#include "view/IStageCanvas.h"

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

void RenderContextStack::Push(IStageCanvas* canvas, RenderContext* render)
{
	Context ctx;
	ctx.canvas = canvas;
	ctx.render = render;
	m_stack.push_back(ctx);
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

	int width, height;
	ctx.render->GetProjection(width, height);
	DrawCallBatching::Instance()->OnSize(width, height);
	ScreenCache::Instance()->SetSize(width, height);
}

void RenderContextStack::SetModelView(const Vector& offset, float scale)
{
	if (m_curr_ctx) {
		m_curr_ctx->SetModelView(offset, scale);
	} else {
		ShaderMgr::Instance()->SetModelView(offset, scale);
	}
}

void RenderContextStack::SetProjection(int width, int height)
{
	if (m_curr_ctx) {
		m_curr_ctx->SetProjection(width, height);
	} else {
		ShaderMgr::Instance()->SetProjection(width, height);
	}
}

bool RenderContextStack::GetModelView(Vector& offset, float& scale) const
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
		m_curr_ctx->GetProjection(width, height);
		return true;
	}
}

}