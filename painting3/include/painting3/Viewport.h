#pragma once

#include <SM_Vector.h>

namespace pt3
{

class Camera;

class Viewport
{
public:
	Viewport();

	void SetSize(float width, float height);

	sm::vec2 TransPos3ProjectToScreen(const sm::vec3& proj, const Camera& cam) const;
	sm::vec3 TransPos3ScreenToDir(const sm::vec2& screen, const Camera& cam) const;

	sm::vec3 MapToSphere(const sm::vec2& touchpoint) const;

private:
	float m_width, m_height;

}; // Viewport

}