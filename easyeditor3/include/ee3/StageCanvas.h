#pragma once

#include <ee/OnePassCanvas.h>

#include <ee0/Observer.h>

#include <node3/Camera.h>
#include <node3/Viewport.h>
#include <node0/SceneNode.h>

namespace ee { class LibraryPanel; }

namespace ee3
{

class StagePage;

class StageCanvas : public ee::OnePassCanvas, public ee0::Observer
{
public:
	StageCanvas(StagePage* stage, wxGLContext* glctx = nullptr,
		bool has2d = false);

	virtual void OnNotify(ee0::MessageID msg, const ee0::VariantSet& variants) override;

	n3::Camera& GetCamera() { return m_camera; }
	const n3::Camera& GetCamera() const { return m_camera; }

	const n3::Viewport& GetViewport() const { return m_viewport; }

	//sm::vec2 TransPos3ProjectToScreen(const sm::vec3& proj) const;
	//sm::vec3 TransPos3ScreenToDir(const sm::vec2& screen) const;

protected:
	virtual void OnSize(int w, int h) override;
	virtual void OnDrawSprites() const override;

	virtual void DrawBackground() const;

private:
	void DrawNodes() const;

private:
	StagePage* m_stage;

	bool m_has2d;

	n3::Camera   m_camera;
	n3::Viewport m_viewport;

	sm::mat4 m_mat_projection;

}; // StageCanvas

}