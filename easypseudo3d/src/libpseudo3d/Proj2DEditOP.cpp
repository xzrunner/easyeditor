#include "Proj2DEditOP.h"

#include <ee/Symbol.h>
#include <ee/Camera.h>
#include <ee/EditPanelImpl.h>
#include <ee/panel_msg.h>
#include <ee/SpriteRenderer.h>
#include <ee/SymbolMgr.h>

namespace epseudo3d
{

Proj2DEditOP::Proj2DEditOP(wxWindow* wnd, ee::EditPanelImpl* stage)
	: ee::EditOP(wnd, stage)
	, m_player(NULL)
{
}

Proj2DEditOP::~Proj2DEditOP()
{
	delete m_player;
}

bool Proj2DEditOP::OnKeyDown(int keyCode)
{
	static const int OFFSET = 10;
	ee::Camera* cam = m_stage->GetCamera();
	switch (keyCode)
	{
	case 'a': case 'A':
		cam->Translate(sm::vec2(-OFFSET, 0));
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
		break;
	case 'd': case 'D':
		cam->Translate(sm::vec2(OFFSET, 0));
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
		break;
	case 's': case 'S':
		cam->Translate(sm::vec2(0, -OFFSET));
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
		break;
	case 'w': case 'W':
		cam->Translate(sm::vec2(0, OFFSET));
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
		break;
	}
	return false;			
}

bool Proj2DEditOP::onDraw() const
{
	if (m_player) {
		ee::Camera* cam = m_stage->GetCamera();
		const sm::vec2& center = cam->GetPosition();
		ee::SpriteRenderer::Instance()->Draw(m_player, sm::mat4(), center);
	}
	return false;
}

void Proj2DEditOP::InitPlayer()
{
	m_player = ee::SymbolMgr::Instance()->FetchSymbol("mmzb/1047_2.png");
}

}