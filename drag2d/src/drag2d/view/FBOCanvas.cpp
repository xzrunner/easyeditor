#include "FBOCanvas.h"

#include "render/ShaderMgr.h"
#include "render/BlendShader.h"
#include "render/ScreenFBO.h"

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

//	glClearColor(0.8f, 0.8f, 0.8f, 1);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//////////////////////////////////////////////////////////////////////////
	// Draw to FBO
	//////////////////////////////////////////////////////////////////////////
 	mgr->sprite();
// 	BlendShader* shader = static_cast<BlendShader*>(mgr->GetSpriteShader());
//  	shader->SetBaseTexID(fbo->GetTexID());
// // 	shader->SetBlendMode("exclusion");
 	mgr->SetFBO(fbo.GetFboID());

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
	mgr->DrawScreen(fbo.GetTexID());
}

}