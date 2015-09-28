#ifndef _TEST_DTEX_TEST_ASYNC_LOAD_H_
#define _TEST_DTEX_TEST_ASYNC_LOAD_H_

#include "TestBase.h"

namespace tdtex
{

class TestAsyncLoad : public TestBase
{
public:
	TestAsyncLoad();
	virtual ~TestAsyncLoad();
	
	virtual void Load();

protected:
	virtual void OnUpdate();

private:
 	std::vector<dtex_package*> m_packages;

}; // TestAsyncLoad

}

#endif // _TEST_DTEX_TEST_ASYNC_LOAD_H_