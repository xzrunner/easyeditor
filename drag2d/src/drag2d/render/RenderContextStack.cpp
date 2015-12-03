#include "RenderContextStack.h"
#include "RenderContext.h"
#include "GL.h"
#include "DrawCallBatching.h"

#include "view/IStageCanvas.h"

namespace d2d
{

RenderContextStack* RenderContextStack::m_instance = NULL;

RenderContextStack* RenderContextStack::Instance()
{
	if (!m_instance) {
		m_instance = new RenderContextStack;
	}
	return m_instance;
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

	Context ctx = m_stack.back();

	ctx.canvas->SetCurrentCanvas();

	int width, height;
	ctx.render->GetProjection(width, height);

	GL::Viewport(0, 0, width, height);
	DrawCallBatching::Instance()->OnSize(width, height);
}

void RenderContextStack::SetModelView(const Vector& offset, float scale)
{
	if (m_stack.empty()) {
		return;
	} else {
		m_stack.back().render->SetModelView(offset, scale);
	}
}

void RenderContextStack::SetProjection(int width, int height)
{
	if (m_stack.empty()) {
		return;
	} else {
		m_stack.back().render->SetProjection(width, height);
	}
}

bool RenderContextStack::GetModelView(Vector& offset, float& scale) const
{
	if (m_stack.empty()) {
		return false;		
	} else {
		return m_stack.back().render->GetModelView(offset, scale);
	}
}

bool RenderContextStack::GetProjection(int& width, int& height) const
{
	if (m_stack.empty()) {
		return false;
	} else {
		return m_stack.back().render->GetProjection(width, height);	
	}
}

}