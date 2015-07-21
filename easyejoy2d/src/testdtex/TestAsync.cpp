#include "TestAsync.h"

#include <dtex_facade.h>
#include <package.h>

namespace tdtex
{

TestAsync::TestAsync()
{
	const char* cfg =
		"{ \n"
		"	\"open_c1\" : false, \n"
		"	\"open_c2\" : true, \n"
		"	\"open_c3\" : true \n"
		"} \n"
		;
	dtexf_create(cfg);
}

TestAsync::~TestAsync()
{
	dtexf_release();

	for_each(m_sprites.begin(), m_sprites.end(), DeletePointerFunctor<eejoy2d::EJSprite>());
}

void TestAsync::Load()
{
//  	LoadSprite("2001dashi", "2001dashi1_run_1", 0.25f);
//  	LoadSprite("2002jumin", "2002jumin1_run_3", 0.25f);
//   	LoadSprite("2003daoke", "2003daoke5_attack2_2", 0.25f);
//   	LoadSprite("2004shenshe", "2004shenshe5_attack1_2", 0.25f);
//  	LoadSprite("2005mojin", "2005mojin3_run_1", 0.25f);
//  	LoadSprite("2006jiabing", "2006jiabing5_attack1_3", 0.25f);
//  	LoadSprite("2007sishi", "2007sishi5_attack1_3", 0.25f);
//   	LoadSprite("2008tianzai", "2008tianzai6_attack1_3", 0.25f);
//   	LoadSprite("2009daoshi", "2009daoshi3_run_3", 0.25f);
   	LoadSprite("2010shennv", "2010shennv4_attack1_2", 0.25f);
//   	LoadSprite("2011dapeng", "2011dapeng1_circlestyle_attack_1", 0.25f);
//   	LoadSprite("2012fujia", "2012fujia3_bomb_1", 0.25f);
//   	LoadSprite("2013baji", "2013baji1_attack2_3", 0.25f);
//   	LoadSprite("2014yingyan", "2014yingyan1_run_3", 0.25f);
//   	LoadSprite("2022xueying", "xueying_ghosttype_attack_1", 0.25f);
//   	LoadSprite("2201yini", "2201yini1_attack1_1", 0.25f);
//   	LoadSprite("2202xiyu", "2202xiyu1_run_3", 0.25f);
//   	LoadSprite("2203langqi", "2203langqi2_cheer_1", 0.25f);
//   	LoadSprite("2204wuzhu", "2204wuzhu1_cheer1_1", 0.25f);
//   	LoadSprite("2205huoyuansu", "2205huoyuansu_summon_1", 0.25f);
//   	LoadSprite("2206jijian", "2206jijian1_attack5_1", 0.25f);
//   	LoadSprite("2208tieta", "2208tieta2_attack1_1", 0.25f);
//   	LoadSprite("2209cunminnv", "cunminnv_run_1", 0.25f);
//   	LoadSprite("2210cunminnan", "cunminnan_run_1", 0.25f);
//   	LoadSprite("2212huojuren", "2212huojuren_attack1_3", 0.25f);	
	dtexf_c3_load_pkg_finish();
}

void TestAsync::Update()
{
	static int count = 0;
	count++;

 	if (count == 50) {
 		dtexf_async_load_spr("2010shennv", "2010shennv4_attack1_2", "2010shennv.epp");
	}

//   	if (count == 50) {
//   		dtexf_async_load_spr("2001dashi", "2001dashi1_run_1", "2001dashi.epp");
//   	} else if (count == 100) {
//   		dtexf_async_load_spr("2002jumin", "2002jumin1_run_3", "2002jumin.epp");
//   	} else if (count == 150) {
//   		dtexf_async_load_spr("2003daoke", "2003daoke5_attack2_2", "2003daoke.epp");
//   	} else if (count == 200) {
//   		dtexf_async_load_spr("2004shenshe", "2004shenshe5_attack1_2", "2004shenshe.epp");
//   	} else if (count == 250) {
//   		dtexf_async_load_spr("2005mojin", "2005mojin3_run_1", "2005mojin.epp");
//   		dtexf_async_load_spr("2006jiabing", "2006jiabing5_attack1_3", "2006jiabing.epp");
//   		dtexf_async_load_spr("2007sishi", "2007sishi5_attack1_3", "2007sishi.epp");
//   	} else if (count == 300) {
//   		dtexf_async_load_spr("2008tianzai", "2008tianzai6_attack1_3", "2008tianzai.epp");
//   		dtexf_async_load_spr("2009daoshi", "2009daoshi3_run_3", "2009daoshi.epp");
//   		dtexf_async_load_spr("2010shennv", "2010shennv4_attack1_2", "2010shennv.epp");
//   		dtexf_async_load_spr("2011dapeng", "2011dapeng1_circlestyle_attack_1", "2011dapeng.epp");
//   		dtexf_async_load_spr("2012fujia", "2012fujia3_bomb_1", "2012fujia.epp");
//   		dtexf_async_load_spr("2013baji", "2013baji1_attack2_3", "2013baji.epp");
//   	} else if (count == 350) {
//   		dtexf_async_load_spr("2014yingyan", "2014yingyan1_run_3", "2014yingyan.epp");
//   		dtexf_async_load_spr("2022xueying", "xueying_ghosttype_attack_1", "2022xueying.epp");
//   		dtexf_async_load_spr("2201yini", "2201yini1_attack1_1", "2201yini.epp");
//   		dtexf_async_load_spr("2202xiyu", "2202xiyu1_run_3", "2202xiyu.epp");
//   		dtexf_async_load_spr("2203langqi", "2203langqi2_cheer_1", "2203langqi.epp");
//   		dtexf_async_load_spr("2204wuzhu", "2204wuzhu1_cheer1_1", "2204wuzhu.epp");
//   		dtexf_async_load_spr("2205huoyuansu", "2205huoyuansu_summon_1", "2205huoyuansu.epp");
//   		dtexf_async_load_spr("2206jijian", "2206jijian1_attack5_1", "2206jijian.epp");
//   		dtexf_async_load_spr("2208tieta", "2208tieta2_attack1_1", "2208tieta.epp");
//   		dtexf_async_load_spr("2209cunminnv", "cunminnv_run_1", "2209cunminnv.epp");
//   		dtexf_async_load_spr("2210cunminnan", "cunminnan_run_1", "2210cunminnan.epp");
//   		dtexf_async_load_spr("2212huojuren", "2212huojuren_attack1_3", "2212huojuren.epp");
//   	}
	dtexf_update();

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Update();
	}
}

void TestAsync::Draw() const
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

void TestAsync::LoadSprite(const std::string& pkg_name, const std::string& anim_name, 
						   float scale)
{
	ej_package* pkg = dtexf_c3_load_pkg(pkg_name.c_str(), (pkg_name + ".epd").c_str(), 1);
	m_ej_pkgs.push_back(pkg);

	eejoy2d::EJSprite* spr = new eejoy2d::EJSprite(pkg, anim_name.c_str());
	m_sprites.push_back(spr);

	dtexf_c3_load_pkg(pkg_name.c_str(), (pkg_name + ".epp").c_str(), scale);	
}

void TestAsync::LoadC2Sprite(const std::string& pkg_name, const std::string& anim)
{
	std::string epd_path = pkg_name + ".epd",
		epp_path = pkg_name + ".epp";

	const char* files[2];
	files[0] = epd_path.c_str();
	files[1] = epp_path.c_str();
	ej_package* pkg = package_load(pkg_name.c_str(), files, 2);

	dtexf_c2_load_begin();
	dtexf_c2_load_sprite(pkg, anim.c_str());
	dtexf_c2_load_end();

	m_sprites.push_back(new eejoy2d::EJSprite(pkg, anim.c_str()));
}

}