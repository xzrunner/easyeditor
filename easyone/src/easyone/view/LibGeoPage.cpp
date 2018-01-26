#include "view/LibGeoPage.h"

#include <ee/LibraryList.h>

#include <easynode3d/Symbol.h>

#include <node3/SurfaceFactory.h>
#include <node3/ModelParametric.h>
#include <node3/ObjectModel.h>
#include <node3/ModelType.h>
#include <node3/AABB.h>
#include <node3/ParametricEquations.h>

namespace eone
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
	auto model = std::unique_ptr<n3::Model>(new n3::ModelParametric(surface, aabb));
	auto obj_model = std::make_shared<n3::ObjectModel>();
	obj_model->SetModel(model);
	obj_model->SetAABB(aabb);

	auto sym = std::make_shared<enode3d::Symbol>();
	sym->SetName(name);
	sym->SetModel(obj_model);

	std::string filepath = name + ".geo";
	obj_model->SetFilepath(filepath);
	sym->SetFilepath(filepath);

//	sym->RefreshThumbnail(sym->GetFilepath());
	m_list->Insert(sym);
}

}