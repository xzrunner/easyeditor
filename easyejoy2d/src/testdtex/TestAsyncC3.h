#ifndef _TEST_DTEX_TEST_ASYNC_C3_H_
#define _TEST_DTEX_TEST_ASYNC_C3_H_

#include "TestBase.h"

#include <vector>
#include <easyejoy2d.h>

namespace tdtex
{

class TestAsyncC3 : public TestBase
{
public:
	TestAsyncC3();
	virtual ~TestAsyncC3();
	
	virtual void Load();

protected:
	virtual void OnUpdate();

}; // TestAsyncC3

}

#endif // _TEST_DTEX_TEST_ASYNC_C3_H_