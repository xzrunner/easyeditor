#ifndef _TEST_DTEX_TEST_RRP_C2C3_PVR_H_
#define _TEST_DTEX_TEST_RRP_C2C3_PVR_H_

#include "ITest.h"

#include <easyejoy2d.h>

namespace tdtex
{

class TestRRP_C2C3_PVR : public ITest
{
public:
	TestRRP_C2C3_PVR();
	virtual ~TestRRP_C2C3_PVR();

	virtual void Load();
	virtual void Update();
	virtual void Draw() const;

private:
	eejoy2d::EJSprite* m_spr;

}; // TestRRP_C2C3_PVR

}

#endif // _TEST_DTEX_TEST_RRP_C2C3_PVR_H_