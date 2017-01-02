#ifndef _EASY3D_STAGE_CANVAS_H_
#define _EASY3D_STAGE_CANVAS_H_

#include "Camera.h"

#include <ee/OnePassCanvas.h>

namespace e3d
{

class StageCanvas : public ee::OnePassCanvas
{
public:
	StageCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage);

	virtual void Refresh();

	Camera& GetCamera3() { return m_camera3; }
	const Camera& GetCamera3() const { return m_camera3; }

	sm::ivec2 TransPos3ProjectToScreen(const sm::vec3& proj) const;

protected:
	virtual void InitGL();
	virtual void OnSize(int w, int h);

private:
	Camera m_camera3;

	sm::mat4 m_mat_projection;

}; // StageCanvas

}

#endif // _EASY3D_S TAGE_CANVAS_H_