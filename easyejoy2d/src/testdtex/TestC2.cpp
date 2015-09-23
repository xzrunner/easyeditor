//#include "TestC2.h"
//
//#include <dtex_facade.h>
//
//namespace tdtex
//{
//
//TestC2::TestC2()
//{
//	const char* cfg =
//		"{ \n"
//		"	\"open_c1\" : false, \n"
//		"	\"open_c2\" : true, \n"
//		"	\"open_c3\" : false \n"
//		"} \n"
//		;
//	dtexf_create(cfg);
//}
//
//TestC2::~TestC2()
//{
//	dtexf_release();
//
//	for_each(m_sprites.begin(), m_sprites.end(), DeletePointerFunctor<eejoy2d::EJSprite>());
//}
//
//void TestC2::Load()
//{
//	LoadSprite("2003daoke", "2003daoke6_attack1_1", "2003daoke6_attack1_2");
//	LoadSprite("2006jiabing", "2006jiabing1_attack1_1", "2006jiabing5_attack1_2");
//	LoadSprite("2010shennv", "2010shennv1_attack1_1", "2010shennv4_run_2");
//	LoadSprite("2204wuzhu", "2204wuzhu1_attack1_2", "2204wuzhu2_run_3");
//}
//
//void TestC2::Update()
//{
//	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
//		m_sprites[i]->Update();
//	}
//}
//
//void TestC2::Draw() const
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
//void TestC2::LoadSprite(const std::string& pkg_name, 
//						const std::string& anim0, 
//						const std::string& anim1)
//{
//	std::string epd_path = pkg_name + ".epd",
//		epp_path = pkg_name + ".epp";
//
//	const char* files[2];
//	files[0] = epd_path.c_str();
//	files[1] = epp_path.c_str();
//	ej_package* pkg = package_load(pkg_name.c_str(), files, 2);
//
//	dtexf_c2_load_begin();
//	dtexf_c2_load_sprite(pkg, anim0.c_str());
//	dtexf_c2_load_sprite(pkg, anim1.c_str());
//	dtexf_c2_load_end();
//
//	m_sprites.push_back(new eejoy2d::EJSprite(pkg, anim0.c_str()));
//	m_sprites.push_back(new eejoy2d::EJSprite(pkg, anim1.c_str()));
//}
//
//}