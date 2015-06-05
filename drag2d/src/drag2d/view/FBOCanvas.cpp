#include "FBOCanvas.h"

#include "render/ShaderMgr.h"
#include "render/BlendShader.h"

namespace d2d
{

FBOCanvas::FBOCanvas(EditPanel* stage)
	: IStageCanvas(stage)
{
}

void FBOCanvas::OnSize(int w, int h)
{
	m_fbo.ChangeSize(w, h);
}

void FBOCanvas::OnDrawWhole() const
{
	ShaderMgr* mgr = ShaderMgr::Instance();

//	glClearColor(0.8f, 0.8f, 0.8f, 1);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//////////////////////////////////////////////////////////////////////////
	// Draw to FBO
	//////////////////////////////////////////////////////////////////////////
	mgr->sprite();
	BlendShader* shader = static_cast<BlendShader*>(mgr->GetSpriteShader());
 	shader->SetBaseTexID(m_fbo.GetTexID());
// 	shader->SetBlendMode("exclusion");
	mgr->SetFBO(m_fbo.GetFboID());

	glClearColor(0.8f, 0.8f, 0.8f, 1);
//	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	OnDrawSprites();

	//////////////////////////////////////////////////////////////////////////
	// Draw to Screen
	//////////////////////////////////////////////////////////////////////////

	mgr->SetFBO(0);
	mgr->SetTexture(0);

	mgr->Screen();
	mgr->DrawScreen(m_fbo.GetTexID());
}

}