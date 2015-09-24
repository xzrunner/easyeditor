#include "TestDirect.h"

#include <easyejoy2d.h>
#include <dtex.h>

namespace tdtex
{

TestDirect::TestDirect()
{
	const char* cfg =
		"{ \n"
		"	\"open_c1\" : false, \n"
		"	\"open_c2\" : false, \n"
		"	\"open_c3\" : false	 \n"
		"} \n"
		;
	dtexf_create(cfg);
}

TestDirect::~TestDirect()
{
	dtexf_release();

	for_each(m_sprites.begin(), m_sprites.end(), DeletePointerFunctor<eejoy2d::EJSprite>());
}

void TestDirect::Load()
{
	m_sprites.push_back(new eejoy2d::EJSprite(LoadPackage("c00001", true), "c00001_attack_1"));
 	m_sprites.push_back(new eejoy2d::EJSprite(LoadPackage("c00002", true), "c00002_attack01_1"));
 	m_sprites.push_back(new eejoy2d::EJSprite(LoadPackage("c00003", true), "c00003_attack01_1"));
}

void TestDirect::Update()
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Update();
	}
}

void TestDirect::Draw() const
{
	if (m_sprites.empty()) {
		return;
	}

	eejoy2d::EJScreen* scr = eejoy2d::EJScreen::Instance();
	assert(scr);
	scr->Bind();
	scr->Clear();

	static const int COL = 9;
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Draw((i%COL+1)*150, (std::floor((float)i/COL)+1)*150);
	}

	scr->UnBind();
}

}