
#include "BodyEditStage.h"

namespace emodeling
{

BodyEditStage::BodyEditStage(wxWindow* parent, wxTopLevelWindow* frame, 
							 libshape::Symbol* symbol)
	: libshape::StagePanel(parent, frame, symbol)
{
}

void BodyEditStage::removeShape(d2d::IShape* shape)
{
	libshape::StagePanel::removeShape(shape);

// 	if (m_sprite && m_sprite->GetUserData())
// 	{
// 		libmodeling::Body* bd = static_cast<libmodeling::Body*>(m_sprite->GetUserData());
// 		for (size_t i = 0, n = bd->fixtures.size(); i < n; ++i)
// 		{
// 			if (bd->fixtures[i]->shape == shape)
// 			{
// 				delete bd->fixtures[i];
// 				bd->fixtures.erase(bd->fixtures.begin() + i);
// 				break;
// 			}
// 		}
// 	}
}

void BodyEditStage::insertShape(d2d::IShape* shape)
{
	libshape::StagePanel::insertShape(shape);

// 	if (m_sprite && m_sprite->GetUserData())
// 	{
// 		libmodeling::Body* bd = static_cast<libmodeling::Body*>(m_sprite->GetUserData());
// 		libmodeling::Fixture* fixture = new libmodeling::Fixture;
// 		fixture->body = bd;
// 		shape->Retain();
// 		fixture->shape = shape;
// 		bd->fixtures.push_back(fixture);
// 	}
}

void BodyEditStage::loadShapes()
{
// 	libshape::StagePanel::loadShapes();
// 
// 	if (m_sprite && m_sprite->GetUserData())
// 	{
// 		libmodeling::Body* bd = static_cast<libmodeling::Body*>(m_sprite->GetUserData());
// 		m_shapes.reserve(bd->fixtures.size());
// 		for (size_t i = 0, n = bd->fixtures.size(); i < n; ++i)
// 		{
// 			d2d::IShape* shape = bd->fixtures[i]->shape;
// 			shape->Retain();
// 			m_shapes.push_back(shape);
// 		}
// 	}
}

}
