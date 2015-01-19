#include "TestPVR.h"

#include <dtex_facade.h>

namespace tdtex
{

TestPVR::TestPVR()
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

TestPVR::~TestPVR()
{
	dtexf_release();
}

void TestPVR::Load()
{
	// only for c3 init tex
	ej_package* pkg = dtexf_c3_load_pkg("2002jumin", "test-dtex/2002jumin.epd", 1);
	m_spr = new eejoy2d::EJSprite(pkg, "2002jumin1_run_1");
	dtexf_c3_load_pkg("2002jumin", "test-dtex/2002jumin.epp", 1);
	dtexf_c3_load_pkg_finish();

	dtexf_test_pvr("test-dtex/buildings21.pvr");
}

void TestPVR::Update()
{
	
}

void TestPVR::Draw() const
{
	eejoy2d::EJShader::Reset();

	eejoy2d::EJScreen* scr = eejoy2d::EJScreen::Instance();
	assert(scr);
	scr->Bind();
	scr->Clear();
	// todo 不画这个，DebugDraw也画不出来，状态转换有问题
	m_spr->Draw(100, 100);
	scr->UnBind();
}

}