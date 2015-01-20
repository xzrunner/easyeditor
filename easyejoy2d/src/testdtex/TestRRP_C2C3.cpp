#include "TestRRP_C2C3.h"

#include <dtex_facade.h>

namespace tdtex
{

TestRRP_C2C3::TestRRP_C2C3()
{
	const char* cfg =
		"{ \n"
		"	\"open_c1\" : false, \n"
		"	\"open_c2\" : true, \n"
		"	\"open_c3\" : true \n"
		"} \n"
		;
	dtexf_create(cfg);
}

TestRRP_C2C3::~TestRRP_C2C3()
{
	dtexf_release();
}

void TestRRP_C2C3::Load()
{	
	// C3
	ej_package* pkg = dtexf_c3_load_pkg("2013baji", "test-dtex/2013baji.epd", 1);
	dtexf_c3_load_pkg("2013baji", "test-dtex/2013baji.epp", 1);
	dtexf_c3_load_pkg_finish();

	// RRP
	dtexf_load_pkg("2013baji", "test-dtex/2013baji.rrp");

	// C2
	dtexf_c2_load_begin();
	dtexf_c2_load_sprite(pkg, "2013baji1_attack1_1");
	dtexf_c2_load_end();

	m_spr = new eejoy2d::EJSprite(pkg, "2013baji1_attack1_1");
}

void TestRRP_C2C3::Update()
{
	m_spr->Update();
}

void TestRRP_C2C3::Draw() const
{
	eejoy2d::EJScreen* scr = eejoy2d::EJScreen::Instance();
	assert(scr);
	scr->Bind();
	scr->Clear();

	m_spr->Draw(100, 100);

	scr->UnBind();
}

}