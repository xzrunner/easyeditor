#include <gl/glew.h>

#include "StageCanvas.h"
#include "ShaderMgr.h"
#include "ViewFrustum.h"

#include <ee/panel_msg.h>
#include <ee/ShaderContext.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>

namespace e3d
{

StageCanvas::StageCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage)
	: ee::OnePassCanvas(stage_wnd, stage)
{
}

void StageCanvas::Refresh()
{
	ShaderMgr::Instance()->SetModelView(m_camera3.GetModelViewMat());

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

ee::ivec2 StageCanvas::TransPos3ProjectToScreen(const ee::vec3& proj) const
{
	ee::mat4 mat_modelview = GetCamera3().GetModelViewMat();
	ee::vec3 v0 = mat_modelview * proj;

	ee::vec3 v1 = m_mat_projection * v0;
	v1.z = v0.z;

	return ViewFrustum::TransPos3ProjectToScreen(v1, m_width, m_height);
}

void StageCanvas::InitGL()
{
//	ee::StageCanvas::InitGL();

	//////////////////////////////////////////////////////////////////////////

	try {
		wxLogDebug(_T("StageCanvas::InitGL()"));

		if (glewInit() != GLEW_OK) {
			exit(1);
		}

		ShaderMgr::Instance()->Null();

		glShadeModel(GL_SMOOTH);
		glEnable(GL_TEXTURE_2D);

		ee::ShaderContext::Reload();
	} catch (ee::Exception& e) {
		ee::ExceptionDlg dlg(m_parent, e);
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
	m_mat_projection = ee::mat4::Perspective(-1, 1, -hh, hh, 
		e3d::Camera::CAM_NEAR, e3d::Camera::CAM_FAR);
}

}