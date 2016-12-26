#include "RenderContext.h"
#include "OrthoCamera.h"
#include "CameraMgr.h"
#include "Pseudo3DCamera.h"

#include <shaderlab/SubjectMVP2.h>
#include <shaderlab/SubjectMVP3.h>

namespace ee
{

RenderContext::RenderContext()
{
	m_mod_scale = 0;
	m_proj_width = m_proj_height = 0;
}

void RenderContext::SetModelView(const sm::vec2& offset, float scale)
{
	m_mod_offset = offset;
	m_mod_scale = scale;

	sl::SubjectMVP2::Instance()->NotifyModelview(offset.x, offset.y, scale, scale);
//	sl::SubjectMVP3::Instance()->NotifyModelview(m_p3d_cam->GetModelViewMat());
}

void RenderContext::SetProjection(int width, int height)
{
	m_proj_width = width;
	m_proj_height = height;

	sl::SubjectMVP2::Instance()->NotifyProjection(width, height);

	Pseudo3DCamera* cam = static_cast<Pseudo3DCamera*>(CameraMgr::Instance()->GetCamera(CameraMgr::PSEUDO3D));
	if (cam) {
		const sm_mat4* mat = cam->GetProjectMat();
		sl::SubjectMVP3::Instance()->NotifyProjection(*(const sm::mat4*)mat);
	}
}

bool RenderContext::GetModelView(sm::vec2& offset, float& scale) const 
{
	if (m_mod_scale == 0) {
		return false;
	} else {
		offset = m_mod_offset; 
		scale = m_mod_scale;
		return true;
	}
}

bool RenderContext::GetProjection(int& width, int& height) const 
{ 
	if (m_proj_width == 0 || m_proj_height == 0) {
		return false;
	} else {
		width = m_proj_width; 
		height = m_proj_height;
		return true;
	}
}

void RenderContext::OnBind()
{
// 	sl::SubjectMVP2::Instance()->NotifyModelview(m_mod_offset.x, m_mod_offset.y, m_mod_scale, m_mod_scale);
// 	//	sl::SubjectMVP3::Instance()->NotifyModelview(m_p3d_cam->GetModelViewMat());
// 
// 	sl::SubjectMVP2::Instance()->NotifyProjection(m_proj_width, m_proj_height);
// 
// 	Pseudo3DCamera* cam = static_cast<Pseudo3DCamera*>(CameraMgr::Instance()->GetCamera(CameraMgr::PSEUDO3D));
// 	if (cam) {
// 		sl::SubjectMVP3::Instance()->NotifyProjection(cam->GetProjectMat());
// 	}

	// update camera
	OrthoCamera* cam2 = static_cast<OrthoCamera*>(CameraMgr::Instance()->GetCamera(CameraMgr::ORTHO));
	cam2->SetPosition(-m_mod_offset);
	cam2->SetScale(1 / m_mod_scale);
}

}