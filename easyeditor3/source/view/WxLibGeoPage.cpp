#include "ee3/WxLibGeoPage.h"

#include <ee0/WxLibraryList.h>

#include <node0/SceneNode.h>
#include <node3/SurfaceFactory.h>
#include <node3/ModelParametric.h>
#include <node3/AABB.h>
#include <node3/ParametricEquations.h>
#include <node3/CompModel.h>
#include <node3/CompAABB.h>

namespace ee3
{

WxLibGeoPage::WxLibGeoPage(wxWindow* parent)
	: ee0::WxLibraryPage(parent, "Geometric", true)
{
	LoadDefaultSymbol();
}

void WxLibGeoPage::LoadDefaultSymbol()
{
	LoadGeometric(n3::Cone::TYPE_NAME);
	LoadGeometric(n3::Sphere::TYPE_NAME);
	LoadGeometric(n3::Torus::TYPE_NAME);
	LoadGeometric(n3::TrefoilKnot::TYPE_NAME);
	LoadGeometric(n3::MobiusStrip::TYPE_NAME);
	LoadGeometric(n3::KleinBottle::TYPE_NAME);
}

void WxLibGeoPage::LoadGeometric(const std::string& name)
{
	n3::Surface* surface = n3::SurfaceFactory::Create(name);

	n3::AABB aabb;
	auto model = std::make_shared<n3::ModelParametric>(surface, aabb);

	auto node = std::make_shared<n0::SceneNode>();

	auto& cmodel = node->AddComponent<n3::CompModel>();
	cmodel.SetModel(model);

	auto& caabb = node->AddComponent<n3::CompAABB>();
	caabb.SetAABB(aabb);

	// todo zz
//	auto sym = std::make_shared<NodeSymbol>();
//	sym->SetName(name);
//	sym->SetNode(node);
//
//	std::string filepath = name + ".geo";
//	//obj_model->SetFilepath(filepath);
//	sym->SetFilepath(filepath);
//
////	sym->RefreshThumbnail(sym->GetFilepath());
//	m_list->Insert(sym);
}

}