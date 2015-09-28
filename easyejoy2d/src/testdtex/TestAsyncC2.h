#ifndef _TEST_DTEX_TEST_ASYNC_C2_H_
#define _TEST_DTEX_TEST_ASYNC_C2_H_

#include "TestBase.h"

namespace tdtex
{

class TestAsyncC2 : public TestBase
{
public:
	TestAsyncC2();
	virtual ~TestAsyncC2();
	
	virtual void Load();

protected:
	virtual void OnUpdate();

private:
 	std::vector<dtex_package*> m_packages;

}; // TestAsyncC2

}

#endif // _TEST_DTEX_TEST_ASYNC_C2_H_