#include "FBOCanvas.h"

#include "render/ShaderMgr.h"
#include "render/BlendShader.h"
#include "render/ScreenFBO.h"
#include "render/SpriteRenderer.h"

#include "render/PrimitiveDraw.h"
#include "render/style_config.h"

namespace d2d
{

FBOCanvas::FBOCanvas(EditPanel* stage)
	: IStageCanvas(stage)
{
}

void FBOCanvas::OnSize(int w, int h)
{
	ScreenFBO::Instance()->GetFBO().ChangeSize(w, h);
}

void FBOCanvas::OnDrawWhole() const
{
	const FBO& fbo = ScreenFBO::Instance()->GetFBO();
	ShaderMgr* mgr = ShaderMgr::Instance();

	SpriteRenderer::Instance()->SetCamera(GetCamera());

	//////////////////////////////////////////////////////////////////////////
	// Draw to FBO
	//////////////////////////////////////////////////////////////////////////
 	mgr->sprite();
 	mgr->SetFBO(fbo.GetFboID());

	glClearColor(m_bg_color.r, m_bg_color.g, m_bg_color.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	OnDrawSprites();

	//////////////////////////////////////////////////////////////////////////
	// Draw to Screen
	//////////////////////////////////////////////////////////////////////////

	mgr->SetFBO(0);
	mgr->SetTexture(0);

	mgr->Screen();
	mgr->DrawScreen(fbo.GetTexID());
}

}