#ifndef _TEST_DTEX_TEST_ASYNC_C1_H_
#define _TEST_DTEX_TEST_ASYNC_C1_H_

#include "TestBase.h"

#include <vector>
#include <easyejoy2d.h>

namespace tdtex
{

class TestAsyncC1 : public TestBase
{
public:
	TestAsyncC1();
	virtual ~TestAsyncC1();
	
	virtual void Load();

protected:
	virtual void OnUpdate();

private:
	dtex_package* m_pkg;
	eejoy2d::EJSprite* m_spr;

}; // TestAsyncC1

}

#endif // _TEST_DTEX_TEST_ASYNC_C1_H_