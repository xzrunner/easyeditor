#pragma once

#include <ee0/WxStageCanvas.h>
#include <ee0/Observer.h>

#include <painting3/Camera.h>
#include <painting3/Viewport.h>
#include <node0/typedef.h>

namespace ee0 { class WxLibraryPanel; }

namespace ee3
{

class WxStagePage;

class WxStageCanvas : public ee0::WxStageCanvas, public ee0::Observer
{
public:
	WxStageCanvas(WxStagePage* stage, const std::shared_ptr<wxGLContext>& glctx = nullptr,
		bool has2d = false);

	virtual void OnNotify(ee0::MessageID msg, const ee0::VariantSet& variants) override;

	pt3::Camera& GetCamera() { return m_camera; }
	const pt3::Camera& GetCamera() const { return m_camera; }

	const pt3::Viewport& GetViewport() const { return m_viewport; }

	//sm::vec2 TransPos3ProjectToScreen(const sm::vec3& proj) const;
	//sm::vec3 TransPos3ScreenToDir(const sm::vec2& screen) const;

protected:
	virtual void OnSize(int w, int h) override;
	virtual void OnDrawSprites() const override;

	virtual void DrawBackground() const;

private:
	void DrawNodes() const;

private:
	WxStagePage* m_stage;

	bool m_has2d;

	pt3::Camera   m_camera;
	pt3::Viewport m_viewport;

	sm::mat4 m_mat_projection;

}; // WxStageCanvas

}