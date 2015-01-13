#ifndef _TEST_DTEX_TEST_C3_H_
#define _TEST_DTEX_TEST_C3_H_

#include "ITest.h"

#include <vector>
#include <dtex_facade.h>

namespace tdtex
{

class TestC3 : public ITest
{
public:
	TestC3();
	virtual ~TestC3();
	
	virtual void Load();

private:
	std::vector<ej_package*> m_ej_pkgs;

}; // TestC3

}

#endif // _TEST_DTEX_TEST_C3_H_