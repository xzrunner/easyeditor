#include "TestBase.h"

namespace tdtex
{

TestBase::~TestBase()
{
	for_each(m_sprites.begin(), m_sprites.end(), DeletePointerFunctor<eejoy2d::EJSprite>());
}

void TestBase::Update()
{
	OnUpdate();

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Update();
	}
}

void TestBase::Draw() const
{
	if (m_sprites.empty()) {
		return;
	}

	eejoy2d::EJScreen* scr = eejoy2d::EJScreen::Instance();
	assert(scr);
	scr->Bind();
	scr->Clear();

	static const int COL = 5;
	static const int EDGE = 200;
	static const int OFFSET_X = -500;
	static const int OFFSET_Y = -500;

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		int x = (i % COL + 1) * EDGE + OFFSET_X;
		int y = (std::floor((float)i / COL) + 1) * EDGE + OFFSET_Y;
		m_sprites[i]->Draw(x, y);
	}

	scr->UnBind();
}

}