#ifndef _TEST_DTEX_TEST_DSPRITE_H_
#define _TEST_DTEX_TEST_DSPRITE_H_

#include "ITest.h"

#include <easyejoy2d.h>

namespace tdtex
{

class TestDSprite : public ITest
{
public:
	TestDSprite();
	virtual ~TestDSprite();

	virtual void Load();
	virtual void Update();
	virtual void Draw() const;

private:
	eejoy2d::EJSprite* m_spr;

	eejoy2d::EJSprite* m_spr2;

}; // TestDSprite

}

#endif // _TEST_DTEX_TEST_DSPRITE_H_