#pragma once

#include "msg/Observer.h"

#include <ee/OnePassCanvas.h>

#include <node3/Camera.h>
#include <node3/Viewport.h>
#include <node3/SceneNode.h>

namespace ee { class LibraryPanel; }

namespace eone
{

class StagePanel;

class StageCanvas : public ee::OnePassCanvas, public Observer
{
public:
	StageCanvas(StagePanel* stage, wxGLContext* glctx = nullptr);

	virtual void OnNotify(MessageID msg, const VariantSet& variants) override;

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
	void DrawNodes() const;
	void DrawNode(const n3::SceneNodePtr& node, const sm::mat4& mt) const;

private:
	StagePanel* m_stage;

	n3::Camera   m_camera;
	n3::Viewport m_viewport;

	sm::mat4 m_mat_projection;

}; // StageCanvas

}