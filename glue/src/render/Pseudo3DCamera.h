#ifndef _GLUE_PSEUDO3D_CAMERA_H_
#define _GLUE_PSEUDO3D_CAMERA_H_

#include "Camera.h"
#include "math/Vector.h"

union sm_mat4;

struct c25_camera;

namespace glue
{

class Pseudo3DCamera : public Camera
{
public:
	Pseudo3DCamera();
	virtual ~Pseudo3DCamera();

	virtual void OnSize(int width, int height);
	virtual void Reset();

	vec2 TransPosScreenToProject(int x, int y, int width, int height) const;
	vec2 TransPosProjectToScreen(const vec3& proj, int width, int height) const;

	const sm_mat4* GetModelViewMat() const;
	const sm_mat4* GetProjectMat() const;

private:
	c25_camera* m_cam;

}; // Pseudo3DCamera

}

#endif // _GLUE_PSEUDO3D_CAMERA_H_