#ifndef _EASYEDITOR_CAMERA_H_
#define _EASYEDITOR_CAMERA_H_

#include "Vector.h"

namespace ee
{

class Camera
{
public:
	Camera(float scale = 1.0f);

	void Reset();

	void Translate(const Vector& offset);
	void Scale(float scale, int x, int y, int width, int height);

	Vector TransPosScreenToProject(int x, int y, int width, int height) const;
	Vector TransPosProjectToScreen(const Vector& proj, int width, int height) const;

	void UpdateModelView() const;

	float GetScale() const { return m_scale; }
	void SetScale(float s) { m_scale = s; }

	const Vector& GetPosition() const { return m_center; }
	void SetPosition(const Vector& pos) { m_center = pos; } 

private:
	Vector m_center;
	float m_scale;

}; // Camera

}

#endif // _EASYEDITOR_CAMERA_H_