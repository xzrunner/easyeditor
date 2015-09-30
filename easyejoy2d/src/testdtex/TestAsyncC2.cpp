#include "TestAsyncC2.h"
#include "Utility.h"

#include <dtex.h>

namespace tdtex
{

TestAsyncC2::TestAsyncC2()
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

TestAsyncC2::~TestAsyncC2()
{
	dtexf_release();
}
	
void TestAsyncC2::Load()
{
	m_packages.push_back(Utility::LoadPackage("c00001", 1));
  	m_packages.push_back(Utility::LoadPackage("c00002", 1));
  	m_packages.push_back(Utility::LoadPackage("c00003", 1));

	m_sprites.push_back(new eejoy2d::EJSprite(m_packages[0], "c00001_attack_1"));
   	m_sprites.push_back(new eejoy2d::EJSprite(m_packages[1], "c00002_attack01_1"));
   	m_sprites.push_back(new eejoy2d::EJSprite(m_packages[2], "c00003_attack01_1"));
}

void TestAsyncC2::OnUpdate()
{
	dtexf_update();

	static int frame = 0;

// 	if (frame == 10) {
//    		wxLogDebug("+++++++++++ Async Load");
//    		int* sprite_ids = new int[1];
//    		sprite_ids[0] = dtex_get_spr_id(m_packages[0], "c00001_attack_1");
//    		dtexf_async_load_texture_with_c2(m_packages[0], sprite_ids, 1);
// 	}

  	if (frame == 100) {
  		wxLogDebug("+++++++++++ Async Load");
  		int* sprite_ids = new int[1];
  		sprite_ids[0] = dtex_get_spr_id(m_packages[0], "c00001_attack_1");
  		dtexf_async_load_texture_with_c2(m_packages[0], sprite_ids, 1);
  	} else if (frame == 200) {
  		wxLogDebug("+++++++++++ Async Load");
  		int* sprite_ids = new int[1];
  		sprite_ids[0] = dtex_get_spr_id(m_packages[1], "c00002_attack01_1");
  		dtexf_async_load_texture_with_c2(m_packages[1], sprite_ids, 1);
  	} else if (frame == 300) {
  		wxLogDebug("+++++++++++ Async Load");
  		int* sprite_ids = new int[1];
  		sprite_ids[0] = dtex_get_spr_id(m_packages[2], "c00003_attack01_1");
  		dtexf_async_load_texture_with_c2(m_packages[2], sprite_ids, 1);
  	}

	++frame;
}

}