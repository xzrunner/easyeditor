#include "ee3/WxLibGeoPage.h"

#include <ee0/WxLibraryList.h>
#include <ee0/WxLibraryItem.h>

#include <painting3/AABB.h>
#include <node0/SceneNode.h>
#include <node3/SurfaceFactory.h>
#include <node3/ModelParametric.h>
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
	std::string filepath = name + ".param";
	m_list->Insert(std::make_shared<ee0::WxLibraryItem>(filepath));
}

}