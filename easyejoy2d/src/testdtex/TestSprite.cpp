#include "TestSprite.h"

#include <easyejoy2d.h>
#include <dtex_facade.h>

namespace tdtex
{

TestSprite::TestSprite()
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

TestSprite::~TestSprite()
{
	dtexf_release();
}

void TestSprite::Load()
{
	{
		ej_package* pkg = dtexf_c3_load_pkg("2002jumin", "test-dtex/2002jumin.epd", 1);
		m_spr = new eejoy2d::EJSprite(pkg, "2002jumin1_run_1");
		dtexf_c3_load_pkg("2002jumin", "test-dtex/2002jumin.epp", 1);
	}
	dtexf_c3_load_pkg_finish();

	m_spr2 = new eejoy2d::EJSprite(dtexf_create_sprite("test-dtex/360.png"));
	m_spr2->Translate(300, 0);
}

void TestSprite::Update()
{
}

void TestSprite::Draw() const
{
	eejoy2d::EJScreen* scr = eejoy2d::EJScreen::Instance();
	assert(scr);
	scr->Bind();
	scr->Clear();

	m_spr->Draw(100, 100);
	m_spr2->Draw(200, 200);

	scr->UnBind();
}

}