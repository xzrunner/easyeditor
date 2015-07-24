#include "TestC3.h"

#include <easyejoy2d.h>
#include <dtex_facade.h>

namespace tdtex
{

TestC3::TestC3()
{
	const char* cfg =
		"{ \n"
		"	\"open_c1\" : false, \n"
		"	\"open_c2\" : false, \n"
		"	\"open_c3\" : true \n"
		"} \n"
		;
	dtexf_create(cfg);
}

TestC3::~TestC3()
{
	dtexf_release();

	for_each(m_sprites.begin(), m_sprites.end(), DeletePointerFunctor<eejoy2d::EJSprite>());
}

void TestC3::Load()
{
// 	{
// 		ej_package* pkg = LoadPackage("2003daoke");
// 		dtexf_c3_load_pkg_finish();
// 
// 		m_sprites.push_back(new eejoy2d::EJSprite(pkg, "2003daoke5_attack2_2"));
// 		m_sprites.push_back(new eejoy2d::EJSprite(pkg, "2003daoke3_cheer_1"));
// 	}

	LoadSprite("2001dashi", "2001dashi1_build2_3");
	LoadSprite("2002jumin", "2002jumin1_run_3");
	LoadSprite("2003daoke", "2003daoke5_attack2_2");
	LoadSprite("2004shenshe", "2004shenshe5_attack1_2");
	LoadSprite("2005mojin", "2005mojin3_run_1");
	LoadSprite("2006jiabing", "2006jiabing5_attack1_3");
	LoadSprite("2007sishi", "2007sishi5_attack1_3");
	LoadSprite("2008tianzai", "2008tianzai6_attack1_3");
	LoadSprite("2009daoshi", "2009daoshi3_run_3");
	LoadSprite("2010shennv", "2010shennv4_attack1_2");
	LoadSprite("2011dapeng", "2011dapeng1_circlestyle_attack_1");
	LoadSprite("2012fujia", "2012fujia3_bomb_1");
	LoadSprite("2013baji", "2013baji1_attack2_3");
	LoadSprite("2014yingyan", "2014yingyan1_run_3");
	LoadSprite("2022xueying", "xueying_ghosttype_attack_1");
	LoadSprite("2201yini", "2201yini1_attack1_1");
	LoadSprite("2202xiyu", "2202xiyu1_run_3");
	LoadSprite("2203langqi", "2203langqi2_cheer_1");
	LoadSprite("2204wuzhu", "2204wuzhu1_cheer1_1");
	LoadSprite("2205huoyuansu", "2205huoyuansu_summon_1");
	LoadSprite("2206jijian", "2206jijian1_attack5_1");
	LoadSprite("2208tieta", "2208tieta2_attack1_1");
	LoadSprite("2209cunminnv", "cunminnv_run_1");
	LoadSprite("2210cunminnan", "cunminnan_run_1");
	LoadSprite("2212huojuren", "2212huojuren_attack1_3");

	LoadSprite("1001_1018_b0007_b0018", "building_1001_lv2");	
 	LoadSprite("1002", "building_1002_lv1");	
 	LoadSprite("1003", "building_1003_lv1");	
 	LoadSprite("1004", "building_1004_lv1");	
 	LoadSprite("1005_1007_1009_1020_1029_1030_1031_1032_1033_1044_1202_b0022", "building_1007_lv1");	
 	LoadSprite("1006", "building_1006_lv7");	
 	LoadSprite("1008", "building_1008_lv1");	
 	LoadSprite("1011", "building_1011_lv3");	
 	LoadSprite("1012", "building_1012_lv5");
 	LoadSprite("1013", "building_1013_lv1");	
 	LoadSprite("1014", "building_1014_lv2");	
 	LoadSprite("1015", "building_1015_lv3");	
 	LoadSprite("1016", "building_1016_lv1");	
 	LoadSprite("1017", "building_1017_lv2");	
 	LoadSprite("1019", "building_1019_lv4");	
 	LoadSprite("1021", "building_1021_lv1");	
 	LoadSprite("1022", "fence_1022_x1");	
 	LoadSprite("1023", "scaffold_1023_x2");	
 	LoadSprite("1024", "broken_wood_1024_x2");	
 	LoadSprite("1025", "broken_stone_1025_x2");	
 	LoadSprite("1026", "broken_gen_1026_x2");	
 	LoadSprite("1027", "broken_home_1027_x4");	
 	LoadSprite("1028", "wall_1028_lv1");	
 	LoadSprite("1034_1035_1036_1037_1038_1039_1040_1041_1042_1043_1045_1046", "obstacle_1034");	
 	LoadSprite("1047_1048_1049_1050_1051_1052_1053_1055_1056_1057_1071_1072", "deco_1047");	
 	LoadSprite("1059_1060_1061_1062_1063_1064", "flag_1059");	
 	LoadSprite("1073_1074_1081_1082_1083_1084_1085_1086_1087_1088_1089_1090_1091_1092_cj", "obstacle_1073");	
 	LoadSprite("1201_1203_1204", "building_1201_lv1");	
 	LoadSprite("b0001_b0002_b0020", "b0001_lord_of_destruction");	
 	LoadSprite("b0003", "b0003_pumpkin");	
 	LoadSprite("b0013_b0016_b0017_b0019_b0021", "b0013");	
 	LoadSprite("flags", "flag_Bahrain");	
 	LoadSprite("others", "biglantern");	

	dtexf_c3_load_pkg_finish();
}

void TestC3::Update()
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Update();
	}
}

void TestC3::Draw() const
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

void TestC3::LoadSprite(const std::string& pkg_name, const std::string& anim_name)
{
	ej_package* pkg = dtexf_c3_load_pkg(pkg_name.c_str(), (pkg_name + ".epd").c_str(), 0.35f);
	m_ej_pkgs.push_back(pkg);

	eejoy2d::EJSprite* spr = new eejoy2d::EJSprite(pkg, anim_name.c_str());
	m_sprites.push_back(spr);

	dtexf_c3_load_pkg(pkg_name.c_str(), (pkg_name + ".epp").c_str(), 0.35f);	
}

ej_package* TestC3::LoadPackage(const std::string& pkg_name)
{
	ej_package* pkg = dtexf_c3_load_pkg(pkg_name.c_str(), (pkg_name + ".epd").c_str(), 0.35f);
	m_ej_pkgs.push_back(pkg);

	dtexf_c3_load_pkg(pkg_name.c_str(), (pkg_name + ".epp").c_str(), 0.35f);	

	return pkg;
}

}