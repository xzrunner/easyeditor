#include "ee0/CameraHelper.h"

#include <sprite2/OrthoCamera.h>
#include <gum/RenderContext.h>

namespace ee0
{

sm::vec2 CameraHelper::TransPosScreenToProject(const s2::Camera& cam, int screen_x, int screen_y)
{
	if (cam.Type() != s2::CAM_ORTHO2D) {
		return sm::vec2(0, 0);
	}

	auto& ortho_cam = dynamic_cast<const s2::OrthoCamera&>(cam);
	int w = gum::RenderContext::Instance()->GetWidth(),
		h = gum::RenderContext::Instance()->GetHeight();
	return ortho_cam.TransPosScreenToProject(screen_x, screen_y, w, h);
}

sm::vec2 CameraHelper::TransPosProjectToScreen(const s2::Camera& cam, const sm::vec2& project)
{
	if (cam.Type() != s2::CAM_ORTHO2D) {
		return sm::vec2(0, 0);
	}

	auto& ortho_cam = dynamic_cast<const s2::OrthoCamera&>(cam);
	int w = gum::RenderContext::Instance()->GetWidth(),
		h = gum::RenderContext::Instance()->GetHeight();
	return ortho_cam.TransPosProjectToScreen(project, w, h);
}

}