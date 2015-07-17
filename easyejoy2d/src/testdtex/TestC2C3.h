#ifndef _TEST_DTEX_TEST_C2C3_H_
#define _TEST_DTEX_TEST_C2C3_H_

#include "ITest.h"

#include <vector>
#include <easyejoy2d.h>

namespace tdtex
{

class TestC2C3 : public ITest
{
public:
	TestC2C3();
	virtual ~TestC2C3();
	
	virtual void Load();
	virtual void Update();
	virtual void Draw() const;

private:
	void LoadSprite(const char* test_data[][3], int count);

private:
	std::vector<eejoy2d::EJSprite*> m_sprites;

}; // TestC2C3

}

#endif // _TEST_DTEX_TEST_C2C3_H_