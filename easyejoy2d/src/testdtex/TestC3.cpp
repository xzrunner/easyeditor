#include "TestC3.h"

namespace tdtex
{

TestC3::TestC3()
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
	ej_package* pkg0 = dtexf_c3_load_pkg("2003daoke", "2003daoke", 1);
	m_ej_pkgs.push_back(pkg0);

	dtexf_c3_load_pkg_finish();
}

}