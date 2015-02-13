#ifndef _TEST_DTEX_TEST_B4R_C3_H_
#define _TEST_DTEX_TEST_B4R_C3_H_

#include "ITest.h"

#include <easyejoy2d.h>

namespace tdtex
{

class TestB4R_C3 : public ITest
{
public:
	TestB4R_C3();
	virtual ~TestB4R_C3();

	virtual void Load();
	virtual void Update();
	virtual void Draw() const;

private:
	eejoy2d::EJSprite* m_spr;

}; // TestB4R_C3

}

#endif // _TEST_DTEX_TEST_B4R_C3_H_