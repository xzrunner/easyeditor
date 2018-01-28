#pragma once

#include <ee/OnePassCanvas.h>

#include <ee0/Observer.h>

#include <memory>

namespace s2 { class Camera; }

namespace ee2
{

class StagePanel;

class StageCanvas : public ee::OnePassCanvas, public ee0::Observer
{
public:
	StageCanvas(StagePanel* stage, wxGLContext* glctx = nullptr);

	virtual void OnNotify(ee0::MessageID msg, const ee0::VariantSet& variants) override;

	std::shared_ptr<s2::Camera>& GetCamera() { return m_cam; }
	const std::shared_ptr<s2::Camera>& GetCamera() const { return m_cam; }

protected:
	virtual void OnSize(int w, int h) override;
	virtual void OnDrawSprites() const override;

	virtual void DrawBackground() const;

private:
	void DrawNodes() const;

private:
	StagePanel* m_stage;

	std::shared_ptr<s2::Camera> m_cam = nullptr;

}; // StageCanvas

}