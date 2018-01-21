#pragma once

#include <ee/OnePassCanvas.h>

#include <node3/Camera.h>
#include <node3/Viewport.h>

namespace ee { class MultiSpritesImpl; }

namespace enode3d
{

class StagePanel;

class StageCanvas : public ee::OnePassCanvas
{
public:
	StageCanvas(StagePanel* stage, wxGLContext* glctx = nullptr, 
		bool has2d = false);

	virtual void RefreshCanvas() override;

	n3::Camera& GetCamera() { return m_camera; }
	const n3::Camera& GetCamera() const { return m_camera; }

	const n3::Viewport& GetViewport() const { return m_viewport; }

	sm::vec2 TransPos3ProjectToScreen(const sm::vec3& proj) const;
	sm::vec3 TransPos3ScreenToDir(const sm::vec2& screen) const;

protected:
	virtual void OnSize(int w, int h) override;
	virtual void OnDrawSprites() const override;

	virtual void DrawBackground() const;

private:
	void DrawSprites() const;

private:
	StagePanel* m_stage;

	bool m_has2d;

	n3::Camera   m_camera;
	n3::Viewport m_viewport;

	sm::mat4 m_mat_projection;

}; // StageCanvas

}