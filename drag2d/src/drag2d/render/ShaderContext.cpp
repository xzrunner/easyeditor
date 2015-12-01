#include "ShaderContext.h"

#include <stdio.h>

#include "ShaderMgrBase.h"
#include "ShaderMgr.h"

namespace d2d
{

ShaderMgrBase* ShaderContext::m_curr = NULL;
ShaderMgrBase* ShaderContext::m_mgr2 = NULL;
ShaderMgrBase* ShaderContext::m_mgr3 = NULL;

void ShaderContext::Reload()
{
	if (m_mgr2) {
		m_mgr2->reload();
	}
	if (m_mgr3) {
		m_mgr3->reload();
	}
}

void ShaderContext::Flush()
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

void ShaderContext::Reset()
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

void ShaderContext::Bind2d()
{
	Bind(m_mgr2);
}

void ShaderContext::Bind3d()
{
	Bind(m_mgr3);
}

void ShaderContext::Bind(ShaderMgrBase* mgr)
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

void ShaderContext::SetModelView(const Vector& offset, float scale)
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
