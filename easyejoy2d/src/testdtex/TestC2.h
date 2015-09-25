#ifndef _TEST_DTEX_TEST_C2_H_
#define _TEST_DTEX_TEST_C2_H_

#include "TestBase.h"

#include <vector>
#include <easyejoy2d.h>

namespace tdtex
{

class TestC2 : public TestBase
{
public:
	TestC2(bool load_from_c3);
	virtual ~TestC2();
	
	virtual void Load();

private:
	void LoadFromPackage();
	void LoadFromC3();

private:
	bool m_load_from_c3;

}; // TestC2

}

#endif // _TEST_DTEX_TEST_C2_H_