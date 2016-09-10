#ifndef _GUM_PSEUDO3D_CAMERA_H_
#define _GUM_PSEUDO3D_CAMERA_H_

#include "Camera.h"

#include <SM_Vector.h>
#include <SM_Matrix.h>

struct c25_camera;

namespace gum
{

class Pseudo3DCamera : public Camera
{
public:
	Pseudo3DCamera();
	virtual ~Pseudo3DCamera();

	virtual void OnSize(int width, int height);
	virtual void Reset();

	sm::vec2 TransPosScreenToProject(int x, int y, int width, int height) const;
	sm::vec2 TransPosProjectToScreen(const sm::vec3& proj, int width, int height) const;

	const sm::mat4* GetModelViewMat() const;
	const sm::mat4* GetProjectMat() const;

private:
	c25_camera* m_cam;

}; // Pseudo3DCamera

}

#endif // _GUM_PSEUDO3D_CAMERA_H_
