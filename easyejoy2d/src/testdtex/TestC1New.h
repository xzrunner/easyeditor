#ifndef _TEST_DTEX_TEST_C1_H_
#define _TEST_DTEX_TEST_C1_H_

#include "TestBase.h"

#include <vector>
#include <easyejoy2d.h>

namespace tdtex
{

class TestC1 : public TestBase
{
public:
	TestC1();
	virtual ~TestC1();
	
	virtual void Load();

protected:
	virtual void OnUpdate();

private:
	dtex_package* m_pkg;
	eejoy2d::EJSprite* m_spr;

}; // TestC1

}

#endif // _TEST_DTEX_TEST_C1_H_