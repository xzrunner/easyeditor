#include "TestAsyncLoad.h"
#include "Utility.h"

#include <dtex.h>

namespace tdtex
{

TestAsyncLoad::TestAsyncLoad()
{
	const char* cfg =
		"{ \n"
		"	\"open_c1\" : false, \n"
		"	\"open_c2\" : false, \n"
		"	\"open_c3\" : false \n"
		"} \n"
		;
	dtexf_create(cfg);
}

TestAsyncLoad::~TestAsyncLoad()
{
	dtexf_release();
}
	
void TestAsyncLoad::Load()
{
	m_packages.push_back(Utility::LoadPackage("c00001", 1.0f));
	m_packages.push_back(Utility::LoadPackage("c00002", 0.5f));
	m_packages.push_back(Utility::LoadPackage("c00003", 0.25f));

	m_sprites.push_back(new eejoy2d::EJSprite(m_packages[0], "c00001_attack_1"));
 	m_sprites.push_back(new eejoy2d::EJSprite(m_packages[1], "c00002_attack01_1"));
 	m_sprites.push_back(new eejoy2d::EJSprite(m_packages[2], "c00003_attack01_1"));
}

void TestAsyncLoad::OnUpdate()
{
	dtexf_update();

	static int frame = 0;

	if (frame == 100) {
		wxLogDebug("+++++++++++ Async Load");
		AsyncLoadTexture(m_packages[0]);
	} else if (frame == 200) {
		wxLogDebug("+++++++++++ Async Load");
		AsyncLoadTexture(m_packages[1]);
	} else if (frame == 300) {
		wxLogDebug("+++++++++++ Async Load");
		AsyncLoadTexture(m_packages[2]);
	}

	++frame;
}

void TestAsyncLoad::AsyncLoadTexture(dtex_package* pkg) 
{
	for (int i = 0; i < pkg->texture_count; ++i) {
		dtexf_async_load_texture(pkg, i);
	}	
}

}