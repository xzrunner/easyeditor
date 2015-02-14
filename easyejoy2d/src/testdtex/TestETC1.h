#ifndef _TEST_DTEX_TEST_ETC1_H_
#define _TEST_DTEX_TEST_ETC1_H_

#include "ITest.h"

#include <easyejoy2d.h>

namespace tdtex
{

class TestETC1 : public ITest
{
public:
	TestETC1();
	virtual ~TestETC1();

	virtual void Load();
	virtual void Update();
	virtual void Draw() const;	

}; // TestETC1

}

#endif // _TEST_DTEX_TEST_ETC1_H_