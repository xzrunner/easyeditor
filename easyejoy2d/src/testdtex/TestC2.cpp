#include "TestC2.h"

#include <dtex_facade.h>

namespace tdtex
{

TestC2::TestC2()
{
	const char* cfg =
		"{ \n"
		"	\"open_c1\" : false, \n"
		"	\"open_c2\" : true, \n"
		"	\"open_c3\" : false \n"
		"} \n"
		;
	dtexf_create(cfg);
}

TestC2::~TestC2()
{
	dtexf_release();

	for_each(m_sprites.begin(), m_sprites.end(), DeletePointerFunctor<eejoy2d::EJSprite>());
}

void TestC2::Load()
{
	dtex_package *pkg1, *pkg2, *pkg3;

	pkg1 = LoadPackage("c00001", true);
	m_sprites.push_back(new eejoy2d::EJSprite(pkg1, "c00001_attack_1"));

	pkg2 = LoadPackage("c00002", true);
	m_sprites.push_back(new eejoy2d::EJSprite(pkg2, "c00002_attack01_1"));

	pkg3 = LoadPackage("c00003", true);
	m_sprites.push_back(new eejoy2d::EJSprite(pkg3, "c00003_attack01_1"));

	dtexf_c2_load_begin();
	dtexf_c2_load(pkg1, "c00001_attack_1");
	dtexf_c2_load(pkg2, "c00002_attack01_1");
	dtexf_c2_load(pkg3, "c00003_attack01_1");
	dtexf_c2_load_end();
}

void TestC2::Update()
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Update();
	}
}

void TestC2::Draw() const
{
 	if (m_sprites.empty()) {
 		return;
 	}

 	eejoy2d::EJScreen* scr = eejoy2d::EJScreen::Instance();
 	assert(scr);
 	scr->Bind();
 	scr->Clear();
 
  	static const int COL = 5;
  	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
  		m_sprites[i]->Draw((i%COL+1)*200, (std::floor((float)i/COL)+1)*200);
  	}
 
 	scr->UnBind();
}

}