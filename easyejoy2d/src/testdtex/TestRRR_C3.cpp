#include "TestRRR_C3.h"

#include <dtex_facade.h>

namespace tdtex
{

TestRRR_C3::TestRRR_C3()
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

TestRRR_C3::~TestRRR_C3()
{
	dtexf_release();
}

void TestRRR_C3::Load()
{	
	ej_package* pkg = dtexf_c3_load_pkg("2013baji", "test-dtex/2013baji.epd", 1);
	dtexf_c3_load_pkg("2013baji", "test-dtex/2013baji.rrr", 1);
	dtexf_c3_load_pkg_finish();

	m_spr = new eejoy2d::EJSprite(pkg, "2013baji1_sleep_1");
}

void TestRRR_C3::Update()
{
	m_spr->Update();
}

void TestRRR_C3::Draw() const
{
	eejoy2d::EJScreen* scr = eejoy2d::EJScreen::Instance();
	assert(scr);
	scr->Bind();
	scr->Clear();

	m_spr->Draw(300, 300);

	scr->UnBind();
}

}