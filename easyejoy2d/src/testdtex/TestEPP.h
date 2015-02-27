#ifndef _TEST_DTEX_TEST_EPP_H_
#define _TEST_DTEX_TEST_EPP_H_

#include "ITest.h"

#include <easyejoy2d.h>

namespace tdtex
{

class TestEPP : public ITest
{
public:
	TestEPP();
	virtual ~TestEPP();

	virtual void Load();
	virtual void Update();
	virtual void Draw() const;	

private:
	std::vector<ej_package*> m_ej_pkgs;

	std::vector<eejoy2d::EJSprite*> m_sprites;

}; // TestEPP

}

#endif // _TEST_DTEX_TEST_EPP_H_