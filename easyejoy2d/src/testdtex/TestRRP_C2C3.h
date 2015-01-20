#ifndef _TEST_DTEX_TEST_RRP_C2C3_H_
#define _TEST_DTEX_TEST_RRP_C2C3_H_

#include "ITest.h"

#include <easyejoy2d.h>

struct ej_package;

namespace tdtex
{

class TestRRP_C2C3 : public ITest
{
public:
	TestRRP_C2C3();
	virtual ~TestRRP_C2C3();

	virtual void Load();
	virtual void Update();
	virtual void Draw() const;

private:
	eejoy2d::EJSprite* m_spr;

}; // TestRRP_C2C3

}

#endif // _TEST_DTEX_TEST_RRP_C2C3_H_