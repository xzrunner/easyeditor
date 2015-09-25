#ifndef _TEST_DTEX_TEST_BASE_H_
#define _TEST_DTEX_TEST_BASE_H_

#include <easyejoy2d.h>

namespace tdtex
{

class TestBase
{
public:
	virtual ~TestBase();

	virtual void Load() = 0;

	void Draw() const;
	void Update();

protected:
	std::vector<eejoy2d::EJSprite*> m_sprites;

}; // TestBase

}

#endif // _TEST_DTEX_TEST_BASE_H_