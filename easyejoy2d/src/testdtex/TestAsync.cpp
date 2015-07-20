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
	LoadSprite("2002jumin", "2002jumin1_run_3", 0.25f);
	dtexf_c3_load_pkg_finish();

//	LoadC2Sprite("2006jiabing", "2006jiabing1_attack1_1");
}

void TestAsync::Update()
{
	static int count = 0;
	count++;
	if (count == 100) {
		dtexf_async_load_spr("2002jumin", "2002jumin1_run_3", "2002jumin.epp");
//		LoadC2Sprite("2003daoke", "2003daoke6_attack1_1");
	}
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