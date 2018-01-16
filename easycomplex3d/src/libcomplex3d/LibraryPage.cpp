#include "LibraryPage.h"

#include <ee/LibraryList.h>

#include <easynode3d/ModelFile.h>
#include <easynode3d/Symbol.h>

#include <node3/ParametricEquations.h>
#include <node3/ModelParametric.h>
#include <node3/ObjectModel.h>
#include <node3/ModelType.h>
#include <node3/AABB.h>
#include <sprite2/SymType.h>

namespace ecomplex3d
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Geometric", true)
{
	InitLayout();
}

bool LibraryPage::IsHandleSymbol(const ee::SymPtr& sym) const
{
	return false;
}

void LibraryPage::LoadDefaultSymbol()
{
	n3::Surface* surface;

	surface = new n3::Cone(2, 1);
	LoadGeometric(surface, "Cone");

	surface = new n3::Sphere(1);
	LoadGeometric(surface, "Sphere");

	surface = new n3::Torus(0.5f, 0.2f);
	LoadGeometric(surface, "Torus");

	surface = new n3::TrefoilKnot(1);
	LoadGeometric(surface, "TrefoilKnot");

	surface = new n3::MobiusStrip(0.2f);
	LoadGeometric(surface, "MobiusStrip");

	surface = new n3::KleinBottle(0.1f);
	LoadGeometric(surface, "KleinBottle");
}

void LibraryPage::LoadGeometric(n3::Surface* surface, const std::string& name)
{
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