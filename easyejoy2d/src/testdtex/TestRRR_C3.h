#ifndef _TEST_DTEX_TEST_RRR_C3_H_
#define _TEST_DTEX_TEST_RRR_C3_H_

#include "ITest.h"

#include <easyejoy2d.h>

namespace tdtex
{

class TestRRR_C3 : public ITest
{
public:
	TestRRR_C3();
	virtual ~TestRRR_C3();

	virtual void Load();
	virtual void Update();
	virtual void Draw() const;

private:
	eejoy2d::EJSprite* m_spr;

}; // TestRRR_C3

}

#endif // _TEST_DTEX_TEST_RRR_C3_H_