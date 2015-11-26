#include "Proj2DEditOP.h"

namespace epseudo3d
{

Proj2DEditOP::Proj2DEditOP(wxWindow* wnd, d2d::EditPanelImpl* stage)
	: d2d::AbstractEditOP(wnd, stage)
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
	d2d::Camera* cam = m_stage->GetCamera();
	switch (keyCode)
	{
	case 'a': case 'A':
		cam->Translate(d2d::Vector(-OFFSET, 0));
		d2d::SetCanvasDirtySJ::Instance()->SetDirty();
		break;
	case 'd': case 'D':
		cam->Translate(d2d::Vector(OFFSET, 0));
		d2d::SetCanvasDirtySJ::Instance()->SetDirty();
		break;
	case 's': case 'S':
		cam->Translate(d2d::Vector(0, -OFFSET));
		d2d::SetCanvasDirtySJ::Instance()->SetDirty();
		break;
	case 'w': case 'W':
		cam->Translate(d2d::Vector(0, OFFSET));
		d2d::SetCanvasDirtySJ::Instance()->SetDirty();
		break;
	}
	return false;			
}

bool Proj2DEditOP::onDraw() const
{
	if (m_player) {
		d2d::Camera* cam = m_stage->GetCamera();
		const d2d::Vector& center = cam->GetPosition();
		d2d::SpriteRenderer::Instance()->Draw(m_player, d2d::Matrix(), center);
	}
	return false;
}

void Proj2DEditOP::InitPlayer()
{
	m_player = d2d::SymbolMgr::Instance()->FetchSymbol("mmzb/1047_2.png");
}

}