#ifndef _TEST_DTEX_TEST_ASYNC_C3_LOAD_H_
#define _TEST_DTEX_TEST_ASYNC_C3_LOAD_H_

#include "TestBase.h"

#include <vector>
#include <easyejoy2d.h>

namespace tdtex
{

class TestAsyncC3Load : public TestBase
{
public:
	TestAsyncC3Load();
	virtual ~TestAsyncC3Load();
	
	virtual void Load();

protected:
	virtual void OnUpdate();

private:
	std::vector<dtex_package*> m_packages;

}; // TestAsyncC3Load

}

#endif // _TEST_DTEX_TEST_ASYNC_C3_LOAD_H_