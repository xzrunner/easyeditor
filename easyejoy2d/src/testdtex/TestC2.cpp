#include "TestC2.h"

#include <dtex_facade.h>
#include <package.h>

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
 	const char* files[2];
 	files[0] = "test-dtex/2003daoke.epd";
 	files[1] = "test-dtex/2003daoke.epp";
 	ej_package* pkg = package_load("2003daoke", files, 2);
 
 	dtexf_c2_load_begin();
 	dtexf_c2_load_sprite(pkg, "2003daoke6_attack1_1");
 	dtexf_c2_load_end();
 
 	m_sprites.push_back(new eejoy2d::EJSprite(pkg, "2003daoke6_attack1_1"));
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
 
  	static const int COL = 9;
  	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
  		m_sprites[i]->Draw((i%COL+1)*100, (std::floor((float)i/COL)+1)*100);
  	}
 
 	scr->UnBind();
}

}