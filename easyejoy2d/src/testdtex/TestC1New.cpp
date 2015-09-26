#include "TestC1New.h"
#include "utility.h"

#include <dtex_facade.h>

namespace tdtex
{

TestC1::TestC1()
{
	const char* cfg =
		"{ \n"
		"	\"open_c1\" : true, \n"
		"	\"open_c2\" : false, \n"
		"	\"open_c3\" : false \n"
		"} \n"
		;
	dtexf_create(cfg);
}

TestC1::~TestC1()
{
	dtexf_release();
}

void TestC1::Load()
{
	m_pkg = Utility::LoadPackage("c00001", true, 1.0f);
	m_spr = new eejoy2d::EJSprite(m_pkg, "c00001_attack_1");

//	m_sprites.push_back();
}

void TestC1::OnUpdate()
{
	m_spr->Update();
	dtexf_c1_update(m_pkg, m_spr->GetEjSpr());
}

}