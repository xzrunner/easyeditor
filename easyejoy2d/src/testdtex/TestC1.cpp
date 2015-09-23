//#include "TestC1.h"
//
//#include <dtex_facade.h>
//
//namespace tdtex
//{
//
//TestC1::TestC1()
//{
//	const char* cfg =
//		"{ \n"
//		"	\"open_c1\" : true, \n"
//		"	\"open_c2\" : false, \n"
//		"	\"open_c3\" : false \n"
//		"} \n"
//		;
//	dtexf_create(cfg);
//}
//
//TestC1::~TestC1()
//{
//	dtexf_release();
//
//	for_each(m_sprites.begin(), m_sprites.end(), DeletePointerFunctor<eejoy2d::EJSprite>());
//}
//
//void TestC1::Load()
//{
//	LoadSprite("2003daoke", "2003daoke5_attack2_2");
//}
//
//void TestC1::Update()
//{
//	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
//		m_sprites[i]->Update();
//	}
//}
//
//void TestC1::Draw() const
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
//	draw_params params;
//	params.x = params.y = 2000;
//	params.color = 0xffffffff;
//	params.additive = 0x00000000;
//	screen_coord screen;
//	sprite_default_coord(&screen);
//	params.screen = &screen;
//	params.mat = NULL;
//
//  	static const int COL = 5;
//  	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
//
//		for (int x = 0; x < 100; ++x) {
//			for (int y = 0; y < 100; ++y) {
//				ej_sprite* ej_spr = m_sprites[i]->GetEjSpr();
//				params.x = 2000 + x * 50;
//				params.y = 2000 + y * 50;
//				dtexf_c1_draw_anim(ej_spr->pack, ej_spr->ani, ej_spr->action, ej_spr->frame, &params);
//			}
//		}
//  	}
//
// 	scr->UnBind();
//}
//
//void TestC1::LoadSprite(const std::string& pkg_name, const std::string& anim)
//{
//	std::string epd_path = pkg_name + ".epd",
//		epp_path = pkg_name + ".epp";
//
//	const char* files[2];
//	files[0] = epd_path.c_str();
//	files[1] = epp_path.c_str();
//	ej_package* pkg = package_load(pkg_name.c_str(), files, 2);
//
//	eejoy2d::EJSprite* spr = new eejoy2d::EJSprite(pkg, anim.c_str());
//	m_sprites.push_back(spr);
//
//	ej_sprite* ej_spr = spr->GetEjSpr();
//	dtexf_c1_load_anim(ej_spr->pack, ej_spr->ani, ej_spr->action);
//}
//
//}