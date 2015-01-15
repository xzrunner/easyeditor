#include "TestC3.h"

#include <easyejoy2d.h>
#include <dtex_facade.h>

namespace tdtex
{

TestC3::TestC3()
	: m_spr(NULL)
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

void TestC3::Load()
{
	ej_package* pkg0 = dtexf_c3_load_pkg("2003daoke", "test-dtex/2003daoke.epd", 1);
	m_ej_pkgs.push_back(pkg0);

	dtexf_c3_load_pkg("2003daoke", "test-dtex/2003daoke.epp", 1);

	dtexf_c3_load_pkg_finish();

	m_spr = new eejoy2d::EJSprite(pkg0, "2003daoke6_attack1_1");
}

void TestC3::Update()
{
	if (m_spr) {
		m_spr->Update();
	}
}

void TestC3::Draw() const
{
	if (m_spr) {
		eejoy2d::EJShader::Reset();
		m_spr->Draw(100, 100);
	}
}

}