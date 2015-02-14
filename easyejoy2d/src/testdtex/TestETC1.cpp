#include "TestETC1.h"

#include <dtex_facade.h>

namespace tdtex
{

TestETC1::TestETC1()
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

TestETC1::~TestETC1()
{
	dtexf_release();
}

void TestETC1::Load()
{
	// only for c3 init tex
	ej_package* pkg = dtexf_c3_load_pkg("2002jumin", "test-dtex/2002jumin.epd", 1);
	dtexf_c3_load_pkg("2002jumin", "test-dtex/2002jumin.epp", 1);
	dtexf_c3_load_pkg_finish();

	dtexf_test_etc1("test-dtex/2006jiabing1.pkm");
}

void TestETC1::Update()
{
	
}

void TestETC1::Draw() const
{	
}

}