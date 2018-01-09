#pragma once

#include <ee/OnePassCanvas.h>

#include <node3/Camera.h>
#include <node3/Viewport.h>

namespace enode3d
{
class StageCanvas : public ee::OnePassCanvas
{
public:
	StageCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage);

	virtual void Refresh();

	n3::Camera& GetCamera() { return m_camera; }
	const n3::Camera& GetCamera() const { return m_camera; }

	const n3::Viewport& GetViewport() const { return m_viewport; }

	sm::vec2 TransPos3ProjectToScreen(const sm::vec3& proj) const;
	sm::vec3 TransPos3ScreenToDir(const sm::vec2& screen) const;

protected:
	virtual void OnSize(int w, int h) override;

private:
	n3::Camera   m_camera;
	n3::Viewport m_viewport;

	sm::mat4 m_mat_projection;

}; // StageCanvas

}