#include "TestC2.h"
#include "Utility.h"

#include <dtex.h>

namespace tdtex
{

TestC2::TestC2()
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

TestC2::~TestC2()
{
	dtexf_release();
}

void TestC2::Load()
{
	dtex_package *pkg1, *pkg2, *pkg3;

	pkg1 = Utility::LoadPackage("c00001", true);
	m_sprites.push_back(new eejoy2d::EJSprite(pkg1, "c00001_attack_1"));

	pkg2 = Utility::LoadPackage("c00002", true);
	m_sprites.push_back(new eejoy2d::EJSprite(pkg2, "c00002_attack01_1"));

	pkg3 = Utility::LoadPackage("c00003", true);
	m_sprites.push_back(new eejoy2d::EJSprite(pkg3, "c00003_attack01_1"));

	dtexf_c2_load_begin();
	dtexf_c2_load(pkg1, "c00001_attack_1");
	dtexf_c2_load(pkg2, "c00002_attack01_1");
	dtexf_c2_load(pkg3, "c00003_attack01_1");
	dtexf_c2_load_end();
}

}