#ifndef _TEST_DTEX_TEST_C3_H_
#define _TEST_DTEX_TEST_C3_H_

#include "TestBase.h"

#include <vector>
#include <easyejoy2d.h>

namespace tdtex
{

class TestC3 : public TestBase
{
public:
	TestC3(bool need_preload_texture);
	virtual ~TestC3();
	
	virtual void Load();

private:
	void LoadWithPreloadTex();
	void LoadWithnotPreloadTex();

private:
	bool m_need_preload_texture;

}; // TestC3

}

#endif // _TEST_DTEX_TEST_C3_H_