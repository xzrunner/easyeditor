#ifndef _TEST_DTEX_INTERFACE_TEST_H_
#define _TEST_DTEX_INTERFACE_TEST_H_

namespace tdtex
{

class ITest
{
public:
	virtual ~ITest() {}
	virtual void Load() = 0;

}; // ITest

}

#endif // _TEST_DTEX_INTERFACE_TEST_H_