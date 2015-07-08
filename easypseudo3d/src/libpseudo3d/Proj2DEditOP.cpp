#include "Proj2DEditOP.h"

namespace epseudo3d
{

Proj2DEditOP::Proj2DEditOP(d2d::EditPanel* stage)
	: d2d::AbstractEditOP(stage)
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
		m_stage->RefreshStage();
		break;
	case 'd': case 'D':
		cam->Translate(d2d::Vector(OFFSET, 0));
		m_stage->RefreshStage();
		break;
	case 's': case 'S':
		cam->Translate(d2d::Vector(0, -OFFSET));
		m_stage->RefreshStage();
		break;
	case 'w': case 'W':
		cam->Translate(d2d::Vector(0, OFFSET));
		m_stage->RefreshStage();
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