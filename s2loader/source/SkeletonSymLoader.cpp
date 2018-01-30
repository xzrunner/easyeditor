#include "s2loader/SkeletonSymLoader.h"
#include "s2loader/EasySkeletonLoader.h"
#include "s2loader/SpineSkeletonLoader.h"
#include "s2loader/ExtendSymFile.h"

#include <sprite2/SkeletonSymbol.h>
#include <gum/FilepathHelper.h>

#include <fstream>

namespace s2loader
{

SkeletonSymLoader::SkeletonSymLoader(s2::SkeletonSymbol& sym,
									 const std::shared_ptr<const SpriteLoader>& spr_loader,
									 const std::shared_ptr<const JointLoader>& joint_loader)
	: m_sym(sym)
	, m_spr_loader(spr_loader)
	, m_joint_loader(joint_loader)
{
}

void SkeletonSymLoader::LoadJson(const CU_STR& filepath)
{
	CU_STR dir = gum::FilepathHelper::Dir(filepath);

	Json::Value val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	int type = ExtendSymFile::GetType(val);
	switch (type)
	{
	case SYM_SPINE:
		{
			SpineSkeletonLoader loader(m_sym, m_spr_loader, m_joint_loader);
			loader.LoadJson(val, dir);
		}
		break;
	case SYM_UNKNOWN:
		{
			EasySkeletonLoader loader(m_sym, m_spr_loader, m_joint_loader);
			loader.LoadJson(val, dir);
		}
		break;
	}
}

}