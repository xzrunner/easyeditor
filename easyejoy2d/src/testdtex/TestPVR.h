#ifndef _TEST_DTEX_TEST_PVR_H_
#define _TEST_DTEX_TEST_PVR_H_

#include "ITest.h"

#include <easyejoy2d.h>

namespace tdtex
{

class TestPVR : public ITest
{
public:
	TestPVR();
	virtual ~TestPVR();

	virtual void Load();
	virtual void Update();
	virtual void Draw() const;	

private:
	eejoy2d::EJSprite* m_spr;

}; // TestPVR

}

#endif // _TEST_DTEX_TEST_PVR_H_