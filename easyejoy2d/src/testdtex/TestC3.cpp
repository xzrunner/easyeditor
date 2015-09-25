#include "TestC3.h"
#include "Utility.h"

#include <easyejoy2d.h>

#include <dtex.h>

//#define	C3_PRELOAD_TEX

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
}

#ifdef C3_PRELOAD_TEX

void TestC3::Load()
{
 	{
 		dtex_package* pkg = Utility::LoadPackage("c00001", true, 0.5f);
 		dtexf_c3_load(pkg, 1);
 
 		m_sprites.push_back(new eejoy2d::EJSprite(pkg, "c00001_attack_1"));
 	}
	{
		dtex_package* pkg = Utility::LoadPackage("c00002", true, 0.25f);
		dtexf_c3_load(pkg, 1);

		m_sprites.push_back(new eejoy2d::EJSprite(pkg, "c00002_attack01_1"));
	}
 	{
 		dtex_package* pkg = Utility::LoadPackage("c00003", true, 0.1f);
 		dtexf_c3_load(pkg, 1);
 
 		m_sprites.push_back(new eejoy2d::EJSprite(pkg, "c00003_attack01_1"));
 	}

	dtexf_c3_load_end();
}

#else

void TestC3::Load()
{
 	{
 		dtex_package* pkg = Utility::LoadPackage("c00001");
 		dtexf_c3_load(pkg, 0.5f);
 
 		m_sprites.push_back(new eejoy2d::EJSprite(pkg, "c00001_attack_1"));
 	}
	{
 		dtex_package* pkg = Utility::LoadPackage("c00002");
 		dtexf_c3_load(pkg, 0.25f);

		m_sprites.push_back(new eejoy2d::EJSprite(pkg, "c00002_attack01_1"));
	}
 	{
 		dtex_package* pkg = Utility::LoadPackage("c00003");
 		dtexf_c3_load(pkg, 0.1f);
 
 		m_sprites.push_back(new eejoy2d::EJSprite(pkg, "c00003_attack01_1"));
 	}

	dtexf_c3_load_end();
}

#endif

}