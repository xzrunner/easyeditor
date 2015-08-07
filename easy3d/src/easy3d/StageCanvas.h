#ifndef _E3D_STAGE_CANVAS_H_
#define _E3D_STAGE_CANVAS_H_

#include <drag2d.h>

#include "Matrix.h"
#include "Camera.h"

namespace e3d
{

class StageCanvas : public d2d::OnePassCanvas
{
public:
	StageCanvas(wxWindow* stage_wnd, d2d::EditPanelImpl* stage);

	virtual void Refresh();

	Camera& GetCamera3() { return m_camera3; }
	const Camera& GetCamera3() const { return m_camera3; }

	ivec2 TransPos3ProjectToScreen(const vec3& proj) const;

protected:
	virtual void InitGL();
	virtual void OnSize(int w, int h);

private:
	Camera m_camera3;

	mat4 m_mat_projection;

}; // StageCanvas

}

#endif // _E3D_S TAGE_CANVAS_H_