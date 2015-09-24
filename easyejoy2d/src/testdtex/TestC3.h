#ifndef _TEST_DTEX_TEST_C3_H_
#define _TEST_DTEX_TEST_C3_H_

#include "ITest.h"

#include <vector>
#include <easyejoy2d.h>

namespace tdtex
{

class TestC3 : public ITest
{
public:
	TestC3();
	virtual ~TestC3();
	
	virtual void Load();
	virtual void Update();
	virtual void Draw() const;

private:
	std::vector<eejoy2d::EJSprite*> m_sprites;

}; // TestC3

}

#endif // _TEST_DTEX_TEST_C3_H_