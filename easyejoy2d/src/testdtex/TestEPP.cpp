#include "TestEPP.h"

#include <dtex_facade.h>

namespace tdtex
{

TestEPP::TestEPP()
{
	const char* cfg =
		"{ \n"
		"	\"open_c1\" : false, \n"
		"	\"open_c2\" : false, \n"
		"	\"open_c3\" : true \n"
		"} \n"
		;
	dtexf_create(cfg);
}

TestEPP::~TestEPP()
{
	dtexf_release();

	for_each(m_sprites.begin(), m_sprites.end(), DeletePointerFunctor<eejoy2d::EJSprite>());
}

void TestEPP::Load()
{
	ej_package* pkg = dtexf_c3_load_pkg("2013baji", "test-dtex/2013baji.epd", 1);
	dtexf_c3_load_pkg("2013baji", "test-dtex/2013baji.epp", 1);
	dtexf_c3_load_pkg_finish();

	m_sprites.push_back(new eejoy2d::EJSprite(pkg, "2013baji1_attack1_1"));
	m_sprites.push_back(new eejoy2d::EJSprite(pkg, "2013baji1_attack1_2"));
	m_sprites.push_back(new eejoy2d::EJSprite(pkg, "2013baji1_attack1_3"));
	m_sprites.push_back(new eejoy2d::EJSprite(pkg, "2013baji1_attack2_1"));
	m_sprites.push_back(new eejoy2d::EJSprite(pkg, "2013baji1_attack2_2"));
	m_sprites.push_back(new eejoy2d::EJSprite(pkg, "2013baji1_attack2_3"));
	m_sprites.push_back(new eejoy2d::EJSprite(pkg, "2013baji1_cheer_1"));
	m_sprites.push_back(new eejoy2d::EJSprite(pkg, "2013baji1_run_1"));
	m_sprites.push_back(new eejoy2d::EJSprite(pkg, "2013baji1_run_2"));
	m_sprites.push_back(new eejoy2d::EJSprite(pkg, "2013baji1_run_3"));
	m_sprites.push_back(new eejoy2d::EJSprite(pkg, "2013baji1_sleep_1"));
	m_sprites.push_back(new eejoy2d::EJSprite(pkg, "2013baji1_stand_1"));
	m_sprites.push_back(new eejoy2d::EJSprite(pkg, "2013baji1_stand_2"));
	m_sprites.push_back(new eejoy2d::EJSprite(pkg, "2013baji1_stand_3"));
}

void TestEPP::Update()
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Update();
	}
}

void TestEPP::Draw() const
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
		m_sprites[i]->Draw((i%COL+1)*100, (std::floor((float)i/COL)+1)*100);
	}

	scr->UnBind();
}

}