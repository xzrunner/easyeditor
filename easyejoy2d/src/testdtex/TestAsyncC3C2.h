#ifndef _TEST_DTEX_TEST_ASYNC_C3C2_H_
#define _TEST_DTEX_TEST_ASYNC_C3C2_H_

#include "TestBase.h"

#include <vector>
#include <easyejoy2d.h>

namespace tdtex
{

class TestAsyncC3C2 : public TestBase
{
public:
	TestAsyncC3C2();
	virtual ~TestAsyncC3C2();
	
	virtual void Load();

protected:
	virtual void OnUpdate();

private:
	std::vector<dtex_package*> m_packages;

}; // TestAsyncC3C2

}

#endif // _TEST_DTEX_TEST_ASYNC_C3C2_H_