#include "s2loader/ModelSymLoader.h"

#include <painting3/AABB.h>
#include <sprite2/ModelSymbol.h>
#include <node3/SurfaceFactory.h>
#include <node3/ModelParametric.h>
#include <node3/AssimpHelper.h>

#include <boost/filesystem.hpp>

namespace s2loader
{

ModelSymLoader::ModelSymLoader(s2::ModelSymbol& sym)
	: m_sym(sym)
{
}

void ModelSymLoader::LoadFromFile(const CU_STR& filepath)
{
	boost::filesystem::path path(filepath.c_str());
	auto ext = boost::filesystem::extension(path);
	if (ext == ".param")
	{
		auto name = path.stem();
		
		n3::Surface* surface = n3::SurfaceFactory::Create(name.string());

		pt3::AABB aabb;
		auto model = std::make_shared<n3::ModelParametric>(surface, aabb);

		m_sym.SetModel(model);
		m_sym.SetAABB(aabb);
	}
	else
	{
		pt3::AABB aabb;
		auto model = n3::AssimpHelper::Load(filepath.c_str(), aabb);

		m_sym.SetModel(model);
		m_sym.SetAABB(aabb);
	}
}

}