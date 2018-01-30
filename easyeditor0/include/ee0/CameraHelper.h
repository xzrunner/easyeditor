#pragma once

namespace s2 { class Camera; }

#include <SM_Vector.h>

namespace ee0
{

class CameraHelper
{
public:
	static sm::vec2 TransPosScreenToProject(const s2::Camera& cam, int screen_x, int screen_y);
	static sm::vec2 TransPosProjectToScreen(const s2::Camera& cam, const sm::vec2& project);

}; // CameraHelper

}