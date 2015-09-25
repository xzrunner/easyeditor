#include "TestLoad.h"
#include "Utility.h"

#include <easyejoy2d.h>
#include <dtex.h>

namespace tdtex
{

TestLoad::TestLoad()
{
	const char* cfg =
		"{ \n"
		"	\"open_c1\" : false, \n"
		"	\"open_c2\" : false, \n"
		"	\"open_c3\" : false	 \n"
		"} \n"
		;
	dtexf_create(cfg);
}

TestLoad::~TestLoad()
{
	dtexf_release();
}

void TestLoad::Load()
{
	m_sprites.push_back(new eejoy2d::EJSprite(Utility::LoadPackage("c00001", true), "c00001_attack_1"));
 	m_sprites.push_back(new eejoy2d::EJSprite(Utility::LoadPackage("c00002", true), "c00002_attack01_1"));
 	m_sprites.push_back(new eejoy2d::EJSprite(Utility::LoadPackage("c00003", true), "c00003_attack01_1"));
}

}