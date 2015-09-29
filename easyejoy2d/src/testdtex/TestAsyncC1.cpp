#include "TestAsyncC1.h"
#include "utility.h"

#include <dtex.h>

namespace tdtex
{

TestAsyncC1::TestAsyncC1()
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

TestAsyncC1::~TestAsyncC1()
{
	dtexf_release();
}

void TestAsyncC1::Load()
{
	m_pkg = Utility::LoadPackage("c00001");
	m_spr = new eejoy2d::EJSprite(m_pkg, "c00001_attack_1");

//	m_sprites.push_back();
}

void TestAsyncC1::OnUpdate()
{
	m_spr->Update();
	dtexf_c1_update(m_pkg, m_spr->GetEjSpr());
}

}