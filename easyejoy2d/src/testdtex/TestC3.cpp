#include "TestC3.h"

#include <easyejoy2d.h>

#include <dtex.h>

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

// 	{
// 		dtex_package* pkg = LoadPackage("ui");
// 		dtexf_c3_load_pkg(pkg, 0.35f);
// 
// 		m_sprites.push_back(new eejoy2d::EJSprite(pkg, "btn_skill_ok"));
// 		m_sprites.push_back(new eejoy2d::EJSprite(pkg, "fighter_mark_arrow"));
// 		m_sprites.push_back(new eejoy2d::EJSprite(pkg, "pic_guide_girl"));
// 	}

	{
		dtex_package* pkg = LoadPackage("c00001");
		dtexf_c3_load_pkg(pkg, 0.35f);

		m_sprites.push_back(new eejoy2d::EJSprite(pkg, "c00001_attack_1"));
	}
	{
		dtex_package* pkg = LoadPackage("c00002");
		dtexf_c3_load_pkg(pkg, 0.35f);

		m_sprites.push_back(new eejoy2d::EJSprite(pkg, "c00002_attack01_1"));
	}
	{
		dtex_package* pkg = LoadPackage("c00003");
		dtexf_c3_load_pkg(pkg, 0.35f);

		m_sprites.push_back(new eejoy2d::EJSprite(pkg, "c00003_attack01_1"));
	}

	dtexf_c3_load_pkg_end();
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

	static const int COL = 9;
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Draw((i%COL+1)*150, (std::floor((float)i/COL)+1)*150);
	}

	scr->UnBind();
}

dtex_package* TestC3::LoadPackage(const std::string& pkg_name)
{
	dtex_package* pkg = dtexf_preload_pkg(pkg_name.c_str(), (pkg_name + ".epe").c_str(), FILE_EPE);

	int idx = 1;
	while (true)
	{
		std::string path = pkg_name	+ "." + d2d::StringTools::ToString(idx++) + ".ept";
		if (!d2d::FilenameTools::IsFileExist(path)) {
			break;
		}
		dtex_package* _pkg = dtexf_preload_pkg(pkg_name.c_str(), path.c_str(), FILE_EPT);
		assert(_pkg == pkg);
	}

	return pkg;
}

}