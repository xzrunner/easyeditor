#include "PreviewCanvas.h"

#include <ee/SpriteRenderer.h>
#include <ee/Sprite.h>

#include <sprite2/Particle3d.h>
#include <sprite2/CameraType.h>

namespace ecomplex
{

PreviewCanvas::PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
							 const std::vector<ee::SprPtr>& sprs,
							 wxGLContext* glctx)
	: ee::CameraCanvas(stage_wnd, stage, s2::CAM_ORTHO2D, glctx)
	, m_sprs(sprs)
{
}

void PreviewCanvas::OnDrawSprites() const
{
	for (auto& spr : m_sprs) {
		ee::SpriteRenderer::Instance()->Draw(spr.get(), s2::RenderParams());
	}

	s2::Particle3d::Instance()->BufferDraw();
}

}