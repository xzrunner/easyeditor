#include "RenderCtxStack.h"

#include <shaderlab.h>

namespace s2
{

SINGLETON_DEFINITION(RenderCtxStack)

RenderCtxStack::RenderCtxStack() 
{
}

RenderCtxStack::~RenderCtxStack() 
{
}

void RenderCtxStack::Push(const RenderCtx& ctx)
{
	BindCtx(ctx);
	m_stack.push_back(ctx);
}

void RenderCtxStack::Pop()
{
	if (m_stack.empty()) {
		return;
	}

	m_stack.pop_back();

	if (!m_stack.empty()) {
		BindCtx(m_stack.back());		
	}
}

const RenderCtx* RenderCtxStack::Top() const
{
	if (m_stack.empty()) {
		return NULL;
	} else {
		return &m_stack[m_stack.size() - 1];
	}
}

void RenderCtxStack::BindCtx(const RenderCtx& ctx)
{
	sl::SubjectMVP2* mvp2 = sl::SubjectMVP2::Instance();
	mvp2->NotifyModelview(ctx.mv_offset.x, ctx.mv_offset.y, ctx.mv_scale, ctx.mv_scale);
	mvp2->NotifyProjection(ctx.proj_width, ctx.proj_height);
}

}