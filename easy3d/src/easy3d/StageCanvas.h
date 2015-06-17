#ifndef _E3D_STAGE_CANVAS_H_
#define _E3D_STAGE_CANVAS_H_

#include <drag2d.h>

#include "Matrix.h"
#include "Camera.h"

namespace e3d
{

class StageCanvas : public d2d::GLCanvas
{
public:
	StageCanvas(d2d::EditPanel* editPanel);

	virtual void Refresh();

	Camera& GetCamera3() { return m_camera3; }
	const Camera& GetCamera3() const { return m_camera3; }

	ivec2 TransPos3ProjectToScreen(const vec3& proj) const;

protected:
	virtual void initGL();
	virtual void onSize(int w, int h);
	virtual void OnDraw();

private:
	Camera m_camera3;

	mat4 m_mat_projection;

}; // StageCanvas

}

#endif // _E3D_S TAGE_CANVAS_H_