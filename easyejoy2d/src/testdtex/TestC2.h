#ifndef _TEST_DTEX_TEST_C2_H_
#define _TEST_DTEX_TEST_C2_H_

#include "ITest.h"

#include <vector>
#include <easyejoy2d.h>

namespace tdtex
{

class TestC2 : public ITest
{
public:
	TestC2();
	virtual ~TestC2();
	
	virtual void Load();
	virtual void Update();
	virtual void Draw() const;

private:
	void LoadSprite(const std::string& pkg_name, 
		const std::string& anim0, const std::string& anim1);

private:
	std::vector<eejoy2d::EJSprite*> m_sprites;

}; // TestC2

}

#endif // _TEST_DTEX_TEST_C2_H_