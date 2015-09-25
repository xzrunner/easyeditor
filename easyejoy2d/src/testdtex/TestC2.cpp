#include "TestC2.h"
#include "Utility.h"

#include <dtex.h>

#define USE_C3

namespace tdtex
{

TestC2::TestC2()
{
#ifdef USE_C3
	const char* cfg =
		"{ \n"
		"	\"open_c1\" : false, \n"
		"	\"open_c2\" : true, \n"
		"	\"open_c3\" : true \n"
		"} \n"
		;
#else
	const char* cfg =
		"{ \n"
		"	\"open_c1\" : false, \n"
		"	\"open_c2\" : true, \n"
		"	\"open_c3\" : false \n"
		"} \n"
		;
#endif
	dtexf_create(cfg);
}

TestC2::~TestC2()
{
	dtexf_release();
}

#ifdef USE_C3

void TestC2::Load()
{
	dtex_package *pkg1, *pkg2, *pkg3;

	// C3
	pkg1 = Utility::LoadPackage("c00001");
	dtexf_c3_load(pkg1, 0.5f);
	m_sprites.push_back(new eejoy2d::EJSprite(pkg1, "c00001_attack_1"));

	pkg2 = Utility::LoadPackage("c00002");
	dtexf_c3_load(pkg2, 0.25f);
	m_sprites.push_back(new eejoy2d::EJSprite(pkg2, "c00002_attack01_1"));

	pkg3 = Utility::LoadPackage("c00003");
	dtexf_c3_load(pkg3, 0.1f);
	m_sprites.push_back(new eejoy2d::EJSprite(pkg3, "c00003_attack01_1"));

	dtexf_c3_load_end();

	// C2
	dtexf_c2_load_begin();
	dtexf_c2_load(pkg1, "c00001_attack_1");
	dtexf_c2_load(pkg2, "c00002_attack01_1");
	dtexf_c2_load(pkg3, "c00003_attack01_1");
	dtexf_c2_load_end();
}

#else

void TestC2::Load()
{
	dtex_package *pkg1, *pkg2, *pkg3;

	pkg1 = Utility::LoadPackage("c00001", true, 1.0f);
	m_sprites.push_back(new eejoy2d::EJSprite(pkg1, "c00001_attack_1"));

	pkg2 = Utility::LoadPackage("c00002", true, 0.5f);
	m_sprites.push_back(new eejoy2d::EJSprite(pkg2, "c00002_attack01_1"));

	pkg3 = Utility::LoadPackage("c00003", true, 0.25f);
	m_sprites.push_back(new eejoy2d::EJSprite(pkg3, "c00003_attack01_1"));

	dtexf_c2_load_begin();
	dtexf_c2_load(pkg1, "c00001_attack_1");
	dtexf_c2_load(pkg2, "c00002_attack01_1");
	dtexf_c2_load(pkg3, "c00003_attack01_1");
	dtexf_c2_load_end();
}

#endif

}