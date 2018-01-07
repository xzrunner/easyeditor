#ifndef _EASY3D_STAGE_CANVAS_H_
#define _EASY3D_STAGE_CANVAS_H_

#include <ee/OnePassCanvas.h>

#include <node3/Camera.h>

namespace e3d
{
class StageCanvas : public ee::OnePassCanvas
{
public:
	StageCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage);

	virtual void Refresh();

	n3::Camera& GetCameraUVN() { return m_cam_uvn; }
	const n3::Camera& GetCameraUVN() const { return m_cam_uvn; }

	sm::vec2 TransPos3ProjectToScreen(const sm::vec3& proj) const;

protected:
	virtual void OnSize(int w, int h) override;

private:
	n3::Camera m_cam_uvn;

	sm::mat4 m_mat_projection;

}; // StageCanvas

}

#endif // _EASY3D_S TAGE_CANVAS_H_