//#include "TestC2C3.h"
//
//#include <dtex_facade.h>
//
//namespace tdtex
//{
//
//TestC2C3::TestC2C3()
//{
//	const char* cfg =
//		"{ \n"
//		"	\"open_c1\" : false, \n"
//		"	\"open_c2\" : true, \n"
//		"	\"open_c3\" : true \n"
//		"} \n"
//		;
//	dtexf_create(cfg);
//}
//
//TestC2C3::~TestC2C3()
//{
//	dtexf_release();
//
//	for_each(m_sprites.begin(), m_sprites.end(), DeletePointerFunctor<eejoy2d::EJSprite>());
//}
//
//void TestC2C3::Load()
//{
//	const char* test_data[][3] = {
//		{"2003daoke", "2003daoke1_attack1_1", "2003daoke3_attack2_3"},
//		{"2007sishi", "2007sishi1_attack1_1", "2007sishi5_run_3"},
//		{"2012fujia", "2012fujia1_attack1_1", "2012fujia3_bomb_2"},
//	};
//	LoadSprite(test_data, 3);
//}
//
//void TestC2C3::Update()
//{
//	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
//		m_sprites[i]->Update();
//	}
//}
//
//void TestC2C3::Draw() const
//{
// 	if (m_sprites.empty()) {
// 		return;
// 	}
//
// 	eejoy2d::EJScreen* scr = eejoy2d::EJScreen::Instance();
// 	assert(scr);
// 	scr->Bind();
// 	scr->Clear();
// 
//  	static const int COL = 5;
//  	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
//  		m_sprites[i]->Draw((i%COL+1)*200, (std::floor((float)i/COL)+1)*200);
//  	}
// 
// 	scr->UnBind();
//}
//
//void TestC2C3::LoadSprite(const char* test_data[][3], int count)
//{
//	std::vector<ej_package*> pkgs(count);
//
//	// C3
//	for (int i = 0; i < count; ++i)
//	{
//		const char* pkg_name = test_data[i][0];
//		std::string epd = std::string(pkg_name) + ".epd",
//			epp = std::string(pkg_name) + ".epp";
//		pkgs[i] = dtexf_c3_load_pkg(pkg_name, epd.c_str(), 0.25);
//		dtexf_c3_load_pkg(pkg_name, epp.c_str(), 0.25);
//	}
//	dtexf_c3_load_pkg_finish();
//
//	// C2
//	dtexf_c2_load_begin();
//	for (int i = 0; i < count; ++i) 
//	{
//		dtexf_c2_load_sprite(pkgs[i], test_data[i][1]);
//		m_sprites.push_back(new eejoy2d::EJSprite(pkgs[i], test_data[i][1]));
//		dtexf_c2_load_sprite(pkgs[i], test_data[i][2]);
//		m_sprites.push_back(new eejoy2d::EJSprite(pkgs[i], test_data[i][2]));
//	}
//	dtexf_c2_load_end();
//}
//
//}