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
// 	{
// 		ej_package* pkg = LoadPackage("2003daoke");
// 		dtexf_c3_load_pkg_finish();
// 
// 		m_sprites.push_back(new eejoy2d::EJSprite(pkg, "2003daoke5_attack2_2"));
// 		m_sprites.push_back(new eejoy2d::EJSprite(pkg, "2003daoke3_cheer_1"));
// 	}


	LoadSprite("2001dashi", "2001dashi1_build2_3");
	LoadSprite("2002jumin", "2002jumin1_run_3");
	LoadSprite("2003daoke", "2003daoke5_attack2_2");
	LoadSprite("2004shenshe", "2004shenshe5_attack1_2");
	LoadSprite("2005mojin", "2005mojin3_run_1");
	LoadSprite("2006jiabing", "2006jiabing5_attack1_3");
	LoadSprite("2007sishi", "2007sishi5_attack1_3");
	LoadSprite("2008tianzai", "2008tianzai6_attack1_3");
	LoadSprite("2009daoshi", "2009daoshi3_run_3");
	LoadSprite("2010shennv", "2010shennv4_attack1_2");
	LoadSprite("2011dapeng", "2011dapeng1_circlestyle_attack_1");
	LoadSprite("2012fujia", "2012fujia3_bomb_1");
	LoadSprite("2013baji", "2013baji1_attack2_3");
	LoadSprite("2014yingyan", "2014yingyan1_run_3");
	LoadSprite("2022xueying", "xueying_ghosttype_attack_1");
	LoadSprite("2201yini", "2201yini1_attack1_1");
	LoadSprite("2202xiyu", "2202xiyu1_run_3");
	LoadSprite("2203langqi", "2203langqi2_cheer_1");
	LoadSprite("2204wuzhu", "2204wuzhu1_cheer1_1");
	LoadSprite("2205huoyuansu", "2205huoyuansu_summon_1");
	LoadSprite("2206jijian", "2206jijian1_attack5_1");
	LoadSprite("2208tieta", "2208tieta2_attack1_1");
	LoadSprite("2209cunminnv", "cunminnv_run_1");
	LoadSprite("2210cunminnan", "cunminnan_run_1");
	LoadSprite("2212huojuren", "2212huojuren_attack1_3");

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

void TestC3::LoadSprite(const std::string& pkg_name, const std::string& anim_name)
{
	ej_package* pkg = dtexf_c3_load_pkg(pkg_name.c_str(), (pkg_name + ".epd").c_str(), 0.25f);
	m_ej_pkgs.push_back(pkg);

	eejoy2d::EJSprite* spr = new eejoy2d::EJSprite(pkg, anim_name.c_str());
	m_sprites.push_back(spr);

	dtexf_c3_load_pkg(pkg_name.c_str(), (pkg_name + ".epp").c_str(), 0.25f);	
}

ej_package* TestC3::LoadPackage(const std::string& pkg_name)
{
	ej_package* pkg = dtexf_c3_load_pkg(pkg_name.c_str(), (pkg_name + ".epd").c_str(), 0.25f);
	m_ej_pkgs.push_back(pkg);

	dtexf_c3_load_pkg(pkg_name.c_str(), (pkg_name + ".epp").c_str(), 0.25f);	

	return pkg;
}

}