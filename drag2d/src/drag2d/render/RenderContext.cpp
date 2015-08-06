#include "RenderContext.h"

#include <stdio.h>

#include "ShaderMgrBase.h"
#include "ShaderMgr.h"

namespace d2d
{

ShaderMgrBase* RenderContext::m_curr = NULL;
ShaderMgrBase* RenderContext::m_mgr2 = NULL;
ShaderMgrBase* RenderContext::m_mgr3 = NULL;

void RenderContext::Reload()
{
	if (m_mgr2) {
		Bind2d();
		m_mgr2->reload();
	}
	if (m_mgr3) {
		Bind3d();
		m_mgr3->reload();
	}
}

void RenderContext::Flush()
{
	if (m_mgr2) {
		Bind2d();
		m_mgr2->Flush();
	}
	if (m_mgr3) {
		Bind3d();
		m_mgr3->Flush();
	}
}

void RenderContext::Reset()
{
	if (m_mgr2) {
		Bind2d();
		m_mgr2->Reset();
	}
	if (m_mgr3) {
		Bind3d();
		m_mgr3->Reset();
	}
}

void RenderContext::Bind2d()
{
	Bind(m_mgr2);
}

void RenderContext::Bind3d()
{
	Bind(m_mgr3);
}

void RenderContext::Bind(ShaderMgrBase* mgr)
{
	if (m_curr == mgr) {
		return;
	}

	if (m_curr) {
		m_curr->UnBind();
	}
	m_curr = mgr;
	if (m_curr) {
		m_curr->Bind();
	}
}

void RenderContext::SetModelView(const Vector& offset, float scale)
{
	if (m_mgr2) {
		static_cast<ShaderMgr*>(m_mgr2)->SetModelView(offset, scale);
	}
// 	if (m_mgr3) {
// 		mat4 scale = mat4::Scale(scale);
// 		mat4 trasn
// 
// 		mat4 mat;
// 		
// 	}
}

}
