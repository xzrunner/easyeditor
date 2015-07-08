#include "TwoPassCanvas.h"
#include "EditPanel.h"

#include "render/ShaderMgr.h"
#include "render/ScreenFBO.h"
#include "render/SpriteRenderer.h"

namespace d2d
{

TwoPassCanvas::TwoPassCanvas(EditPanel* stage)
	: OnePassCanvas(stage)
{
	stage->AddRefreshObserver(&m_dirty_state);
}

void TwoPassCanvas::OnSize(int w, int h)
{
	ScreenFBO::Instance()->GetFBO().ChangeSize(w, h);
}

void TwoPassCanvas::OnDrawWhole() const
{
	const FBO& fbo = ScreenFBO::Instance()->GetFBO();
	ShaderMgr* mgr = ShaderMgr::Instance();

	SpriteRenderer::Instance()->SetCamera(GetCamera());

	//////////////////////////////////////////////////////////////////////////
	// Draw to FBO
	//////////////////////////////////////////////////////////////////////////
	if (m_dirty_state.IsDirty()) {
 		mgr->SetFBO(fbo.GetFboID());
		OnePassCanvas::OnDrawWhole();
		m_dirty_state.ResetDirty();
	}

	//////////////////////////////////////////////////////////////////////////
	// Draw to Screen
	//////////////////////////////////////////////////////////////////////////

	mgr->SetFBO(0);
	mgr->SetTexture(0);

	mgr->Screen();
	mgr->DrawScreen(fbo.GetTexID());
}

}