#ifndef _TEST_DTEX_TEST_SPRITE_H_
#define _TEST_DTEX_TEST_SPRITE_H_

#include "ITest.h"

#include <easyejoy2d.h>

namespace tdtex
{

class TestSprite : public ITest
{
public:
	TestSprite();
	virtual ~TestSprite();

	virtual void Load();
	virtual void Update();
	virtual void Draw() const;

private:
	eejoy2d::EJSprite* m_spr;

	eejoy2d::EJSprite* m_spr2;

}; // TestSprite

}

#endif // _TEST_DTEX_TEST_SPRITE_H_