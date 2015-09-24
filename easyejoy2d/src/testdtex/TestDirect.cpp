#include "TestDirect.h"

#include <easyejoy2d.h>
#include <dtex.h>

namespace tdtex
{

TestDirect::TestDirect()
{
	const char* cfg =
		"{ \n"
		"	\"open_c1\" : false, \n"
		"	\"open_c2\" : false, \n"
		"	\"open_c3\" : false	 \n"
		"} \n"
		;
	dtexf_create(cfg);
}

TestDirect::~TestDirect()
{
	dtexf_release();

	for_each(m_sprites.begin(), m_sprites.end(), DeletePointerFunctor<eejoy2d::EJSprite>());
}

void TestDirect::Load()
{
	dtex_package* pkg = LoadPackage("ui");
	eejoy2d::EJSprite* spr = new eejoy2d::EJSprite(pkg, "zztest");
	m_sprites.push_back(spr);
}

void TestDirect::Update()
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Update();
	}
}

void TestDirect::Draw() const
{
	if (m_sprites.empty()) {
		return;
	}

	eejoy2d::EJScreen* scr = eejoy2d::EJScreen::Instance();
	assert(scr);
	scr->Bind();
	scr->Clear();

	static const int COL = 9;
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Draw((i%COL+1)*150, (std::floor((float)i/COL)+1)*150);
	}

	scr->UnBind();
}

dtex_package* TestDirect::LoadPackage(const std::string& pkg_name)
{
	dtex_package* pkg = dtexf_preload_pkg(pkg_name.c_str(), (pkg_name + ".epe").c_str(), FILE_EPE);

	int idx = 1;
	while (true)
	{
		std::string path = pkg_name	+ "." + d2d::StringTools::ToString(idx++) + ".ept";
		if (!d2d::FilenameTools::IsFileExist(path)) {
			break;
		}
		dtex_package* _pkg = dtexf_preload_pkg(pkg_name.c_str(), path.c_str(), FILE_EPT);
		assert(_pkg == pkg);
	}

	for (int i = 0; i < pkg->tex_size; ++i) {
		dtexf_load_texture(pkg, i);
	}

	return pkg;
}

}