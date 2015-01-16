#include "TestC3.h"

#include <easyejoy2d.h>
#include <dtex_facade.h>

namespace tdtex
{

TestC3::TestC3()
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

TestC3::~TestC3()
{
	dtexf_release();

	for_each(m_sprites.begin(), m_sprites.end(), DeletePointerFunctor<eejoy2d::EJSprite>());
}

void TestC3::Load()
{
	{
		ej_package* pkg = dtexf_c3_load_pkg("2002jumin", "test-dtex/2002jumin.epd", 1);
		m_ej_pkgs.push_back(pkg);

		eejoy2d::EJSprite* spr = new eejoy2d::EJSprite(pkg, "2002jumin1_run_1");
		m_sprites.push_back(spr);

		dtexf_c3_load_pkg("2002jumin", "test-dtex/2002jumin.epp", 1);
	}
	{
		ej_package* pkg = dtexf_c3_load_pkg("2003daoke", "test-dtex/2003daoke.epd", 1);
		m_ej_pkgs.push_back(pkg);

		eejoy2d::EJSprite* spr = new eejoy2d::EJSprite(pkg, "2003daoke6_attack1_1");
		m_sprites.push_back(spr);

		dtexf_c3_load_pkg("2003daoke", "test-dtex/2003daoke.epp", 1);
	}
	{
		ej_package* pkg = dtexf_c3_load_pkg("2004shenshe", "test-dtex/2004shenshe.epd", 1);
		m_ej_pkgs.push_back(pkg);

		eejoy2d::EJSprite* spr = new eejoy2d::EJSprite(pkg, "2004shenshe1_attack1_1");
		m_sprites.push_back(spr);

		dtexf_c3_load_pkg("2004shenshe", "test-dtex/2004shenshe.epp", 1);	
	}
	{
		ej_package* pkg = dtexf_c3_load_pkg("2005mojin", "test-dtex/2005mojin.epd", 1);
		m_ej_pkgs.push_back(pkg);

		eejoy2d::EJSprite* spr = new eejoy2d::EJSprite(pkg, "2005mojin1_attack1_1");
		m_sprites.push_back(spr);

		dtexf_c3_load_pkg("2005mojin", "test-dtex/2005mojin.epp", 1);	
	}
	{
		ej_package* pkg = dtexf_c3_load_pkg("2006jiabing", "test-dtex/2006jiabing.epd", 1);
		m_ej_pkgs.push_back(pkg);

		eejoy2d::EJSprite* spr = new eejoy2d::EJSprite(pkg, "2006jiabing6_attack1_1");
		m_sprites.push_back(spr);

		dtexf_c3_load_pkg("2006jiabing", "test-dtex/2006jiabing.epp", 1);	
	}
	{
		ej_package* pkg = dtexf_c3_load_pkg("2007sishi", "test-dtex/2007sishi.epd", 1);
		m_ej_pkgs.push_back(pkg);

		eejoy2d::EJSprite* spr = new eejoy2d::EJSprite(pkg, "2007sishi6_attack1_3");
		m_sprites.push_back(spr);

		dtexf_c3_load_pkg("2007sishi", "test-dtex/2007sishi.epp", 1);	
	}
	dtexf_c3_load_pkg_finish();
}

void TestC3::Update()
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Update();
	}
}

void TestC3::Draw() const
{
	if (m_sprites.empty()) {
		return;
	}

	eejoy2d::EJShader::Reset();

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