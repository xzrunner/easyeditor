#ifndef _TEST_DTEX_TEST_DIRECT_H_
#define _TEST_DTEX_TEST_DIRECT_H_

#include "ITest.h"

#include <vector>
#include <easyejoy2d.h>

namespace tdtex
{

class TestDirect : public ITest
{
public:
	TestDirect();
	virtual ~TestDirect();
	
	virtual void Load();
	virtual void Update();
	virtual void Draw() const;

private:
	dtex_package* LoadPackage(const std::string& pkg);

private:
	std::vector<eejoy2d::EJSprite*> m_sprites;

}; // TestDirect

}

#endif // _TEST_DTEX_TEST_DIRECT_H_