#include "ee3/LibGeoPage.h"
#include "ee3/NodeSymbol.h"

#include <ee/LibraryList.h>

#include <node3/SurfaceFactory.h>
#include <node3/ModelParametric.h>
#include <node3/AABB.h>
#include <node3/ParametricEquations.h>
#include <node3/CompModel.h>
#include <node3/CompAABB.h>

namespace ee3
{

LibGeoPage::LibGeoPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Geometric", true)
{
	InitLayout();

	LoadDefaultSymbol();
}

bool LibGeoPage::IsHandleSymbol(const ee::SymPtr& sym) const
{
	return false;
}

void LibGeoPage::LoadDefaultSymbol()
{
	LoadGeometric(n3::Cone::TYPE_NAME);
	LoadGeometric(n3::Sphere::TYPE_NAME);
	LoadGeometric(n3::Torus::TYPE_NAME);
	LoadGeometric(n3::TrefoilKnot::TYPE_NAME);
	LoadGeometric(n3::MobiusStrip::TYPE_NAME);
	LoadGeometric(n3::KleinBottle::TYPE_NAME);
}

void LibGeoPage::LoadGeometric(const std::string& name)
{
	n3::Surface* surface = n3::SurfaceFactory::Create(name);

	n3::AABB aabb;
	auto model = std::make_shared<n3::ModelParametric>(surface, aabb);

	auto node = std::make_shared<n0::SceneNode>();

	auto& cmodel = node->AddComponent<n3::CompModel>();
	cmodel.SetModel(model);

	auto& caabb = node->AddComponent<n3::CompAABB>();
	caabb.SetAABB(aabb);

	auto sym = std::make_shared<NodeSymbol>();
	sym->SetName(name);
	sym->SetNode(node);

	std::string filepath = name + ".geo";
	//obj_model->SetFilepath(filepath);
	sym->SetFilepath(filepath);

//	sym->RefreshThumbnail(sym->GetFilepath());
	m_list->Insert(sym);
}

}