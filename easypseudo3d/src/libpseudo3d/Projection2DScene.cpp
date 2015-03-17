#include "Projection2DScene.h"

namespace epseudo3d
{

Projection2DScene::Projection2DScene()
{
	
}

Projection2DScene::~Projection2DScene()
{

}

void Projection2DScene::Load(const char* filename)
{
	d2d::SymbolMgr* sym_mgr = d2d::SymbolMgr::Instance();
	d2d::SpriteFactory* spr_fac = d2d::SpriteFactory::Instance();
	{
		d2d::ISymbol* sym = sym_mgr->fetchSymbol("mmzb/1002_3_lv5.png");
		d2d::ISprite* spr = spr_fac->create(sym);
		spr->translate(d2d::Vector(100, 100));
		m_sprites.push_back(spr);
	}
}

void Projection2DScene::Draw() const
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
 		d2d::ISprite* spr = m_sprites[i];
		d2d::SpriteDraw::drawSprite(spr);
	}
}

}