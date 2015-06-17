#include <gl/glew.h>

#include "StageCanvas.h"
#include "ShaderMgr.h"
#include "ViewFrustum.h"

namespace e3d
{

StageCanvas::StageCanvas(d2d::EditPanel* editPanel)
	: d2d::GLCanvas(editPanel)
{
}

void StageCanvas::Refresh()
{
	ShaderMgr::Instance()->SetModelView(m_camera3.GetModelViewMat());

	d2d::GLCanvas::Refresh();
}

ivec2 StageCanvas::TransPos3ProjectToScreen(const vec3& proj) const
{
	mat4 mat_modelview = GetCamera3().GetModelViewMat();
	vec3 v0 = mat_modelview * proj;

	vec3 v1 = m_mat_projection * v0;
	v1.z = v0.z;

	return ViewFrustum::TransPos3ProjectToScreen(v1, m_screen_width, m_screen_height);
}

void StageCanvas::initGL()
{
//	d2d::GLCanvas::initGL();

	//////////////////////////////////////////////////////////////////////////

	try {
		wxLogDebug(_T("GLCanvas::initGL()"));

		if (glewInit() != GLEW_OK) {
			exit(1);
		}

		ShaderMgr::Instance()->Null();

		resetViewport();

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

void StageCanvas::onSize(int w, int h)
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

void StageCanvas::OnDraw()
{

}

}