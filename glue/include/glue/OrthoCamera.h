#ifndef _GLUE_ORTHO_CAMERA_H_
#define _GLUE_ORTHO_CAMERA_H_

#include "Camera.h"

#include <SM_Vector.h>

namespace glue
{

class OrthoCamera
{
public:
	OrthoCamera();
	
	virtual void OnSize(int width, int height);
	virtual void Reset();

	sm::vec2 TransPosScreenToProject(int x, int y, int width, int height) const;
	sm::vec2 TransPosProjectToScreen(const sm::vec2& proj, int width, int height) const;

	void Translate(const sm::vec2& offset);
	void Scale(float scale, int x, int y, int width, int height);

	float GetScale() const { return m_scale; }
	void SetScale(float s) { m_scale = s; }

	const sm::vec2& GetPosition() const { return m_position; }
	void SetPosition(const sm::vec2& pos) { m_position = pos; } 

private:
	sm::vec2 m_position;
	float m_scale;

}; // OrthoCamera

}

#endif // _GLUE_ORTHO_CAMERA_H_