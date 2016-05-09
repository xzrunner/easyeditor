#ifndef _EASYEDITOR_ORTHO_CAMERA_H_
#define _EASYEDITOR_ORTHO_CAMERA_H_

#include "Camera.h"

namespace ee
{

class OrthoCamera : public Camera
{
public:
	OrthoCamera(float scale = 1.0f);

	virtual std::string Type() const { return "ortho"; }
	virtual void Reset();

	virtual sm::vec2 TransPosScreenToProject(int x, int y, int width, int height) const;
	virtual sm::vec2 TransPosProjectToScreen(const sm::vec2& proj, int width, int height) const;

	virtual void UpdateModelView() const;

	virtual float GetScale() const { return m_scale; }
	virtual const sm::vec2& GetPosition() const { return m_center; }

	void Translate(const sm::vec2& offset);
	void Scale(float scale, int x, int y, int width, int height);

	void SetScale(float s) { m_scale = s; }
	void SetPosition(const sm::vec2& pos) { m_center = pos; } 

private:
	sm::vec2 m_center;
	float m_scale;

}; // OrthoCamera

}

#endif // _EASYEDITOR_ORTHO_CAMERA_H_