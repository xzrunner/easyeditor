#include <gl/glew.h>

#include "StageCanvas.h"
#include "ShaderMgr.h"
#include "ViewFrustum.h"

namespace e3d
{

StageCanvas::StageCanvas(d2d::EditPanel* editPanel)
	: d2d::OnePassCanvas(editPanel)
{
}

void StageCanvas::Refresh()
{
	ShaderMgr::Instance()->SetModelView(m_camera3.GetModelViewMat());

	d2d::IStageCanvas::RefreshStage();
}

ivec2 StageCanvas::TransPos3ProjectToScreen(const vec3& proj) const
{
	mat4 mat_modelview = GetCamera3().GetModelViewMat();
	vec3 v0 = mat_modelview * proj;

	vec3 v1 = m_mat_projection * v0;
	v1.z = v0.z;

	return ViewFrustum::TransPos3ProjectToScreen(v1, m_width, m_height);
}

void StageCanvas::InitGL()
{
//	d2d::IStageCanvas::InitGL();

	//////////////////////////////////////////////////////////////////////////

	try {
		wxLogDebug(_T("IStageCanvas::InitGL()"));

		if (glewInit() != GLEW_OK) {
			exit(1);
		}

		ShaderMgr::Instance()->Null();

		ResetViewport();

		glShadeModel(GL_SMOOTH);
		glEnable(GL_TEXTURE_2D);

		if (d2d::RenderContext::SHADER_MGR) {
			d2d::RenderContext::SHADER_MGR->reload();
		}	
	} catch (d2d::Exception& e) {
		d2d::ExceptionDlg dlg(m_parent, e);
		dlg.ShowModal();	
	}

	//////////////////////////////////////////////////////////////////////////

	ShaderMgr::Instance()->SetModelView(m_camera3.GetModelViewMat());
}

void StageCanvas::OnSize(int w, int h)
{
	glViewport(0, 0, w, h);
	m_screen.SetSize(w, h);
	m_screen.SetCamera();

	m_camera3.SetScreenSize(w, h);

	ShaderMgr::Instance()->SetProjection(w, h);
	ShaderMgr::Instance()->SetModelView(m_camera3.GetModelViewMat());

	float hh = 1.0f * h / w;
	m_mat_projection = mat4::Perspective(-1, 1, -hh, hh, 
		e3d::Camera::CAM_NEAR, e3d::Camera::CAM_FAR);
}

}