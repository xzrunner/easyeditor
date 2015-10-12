#include "TestAsyncC3Load.h"
#include "Utility.h"

#include <easyejoy2d.h>

#include <dtex.h>

namespace tdtex
{

TestAsyncC3Load::TestAsyncC3Load()
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

TestAsyncC3Load::~TestAsyncC3Load()
{
	dtexf_release();
}

// void TestAsyncC3Load::Load()
// {
// 	m_packages.push_back(Utility::LoadPackage("c00001", 1, false, 2));
// 	m_packages.push_back(Utility::LoadPackage("c00002", 1, false, 2));
// 	m_packages.push_back(Utility::LoadPackage("c00003", 1, false, 2));
// 
// 	m_sprites.push_back(new eejoy2d::EJSprite(m_packages[0], "c00001_attack_1"));
// 	m_sprites.push_back(new eejoy2d::EJSprite(m_packages[1], "c00002_attack01_1"));
// 	m_sprites.push_back(new eejoy2d::EJSprite(m_packages[2], "c00003_attack01_1"));
// 
// 	dtexf_c3_load(m_packages[0], 1.0f);
// 	dtexf_c3_load(m_packages[1], 1.0f);
// 	dtexf_c3_load(m_packages[2], 1.0f);
// 
// 	dtexf_c3_load_end(true);
// }

void TestAsyncC3Load::Load()
{
	m_packages.push_back(Utility::LoadPackage("c00001", 1));
	m_packages.push_back(Utility::LoadPackage("c00002", 1));
	m_packages.push_back(Utility::LoadPackage("c00003", 1));

	m_sprites.push_back(new eejoy2d::EJSprite(m_packages[0], "c00001_attack_1"));
	m_sprites.push_back(new eejoy2d::EJSprite(m_packages[1], "c00002_attack01_1"));
	m_sprites.push_back(new eejoy2d::EJSprite(m_packages[2], "c00003_attack01_1"));

	dtexf_c3_load(m_packages[0], 0.1f);
	dtexf_c3_load(m_packages[1], 0.1f);
	dtexf_c3_load(m_packages[2], 0.1f);

	dtexf_c3_load_end(true);
}

void TestAsyncC3Load::OnUpdate()
{
	dtexf_update();

 	static int frame = 0;

// 	if (frame == 100) {
// 		wxLogDebug("+++++++++++ Async Load");
// 		int* sprite_ids = new int[1];
// 		sprite_ids[0] = dtex_get_spr_id(m_packages[0], "c00003_attack01_1");
// 		dtexf_async_load_texture_from_c3(m_packages[0], sprite_ids, 1);
// 	}

 	if (frame == 100) {
 		wxLogDebug("+++++++++++ Async Load");
 		int* sprite_ids = new int[1];
 		sprite_ids[0] = dtex_get_spr_id(m_packages[0], "c00001_attack_1");
 		dtexf_async_load_texture_from_c3(m_packages[0], sprite_ids, 1);
 	} else if (frame == 200) {
 		wxLogDebug("+++++++++++ Async Load");
 		int* sprite_ids = new int[1];
 		sprite_ids[0] = dtex_get_spr_id(m_packages[1], "c00002_attack01_1");
 		dtexf_async_load_texture_from_c3(m_packages[1], sprite_ids, 1);
 	} else if (frame == 300) {
 		wxLogDebug("+++++++++++ Async Load");
 		int* sprite_ids = new int[1];
 		sprite_ids[0] = dtex_get_spr_id(m_packages[2], "c00003_attack01_1");
 		dtexf_async_load_texture_from_c3(m_packages[2], sprite_ids, 1);
 	}
 
 	++frame;
}

}