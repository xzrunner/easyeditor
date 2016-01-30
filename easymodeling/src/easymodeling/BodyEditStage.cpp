
#include "BodyEditStage.h"

namespace emodeling
{

BodyEditStage::BodyEditStage(wxWindow* parent, wxTopLevelWindow* frame, 
							 libshape::Symbol* symbol)
	: libshape::StagePanel(parent, frame, symbol)
{
}

//bool BodyEditStage::InsertShape(d2d::Shape* shape)
//{
//	return libshape::StagePanel::InsertShape(shape);
//
// 	if (m_sprite && m_sprite->GetUserData())
// 	{
// 		libmodeling::Body* bd = static_cast<libmodeling::Body*>(m_sprite->GetUserData());
// 		libmodeling::Fixture* fixture = new libmodeling::Fixture;
// 		fixture->body = bd;
// 		shape->Retain();
// 		fixture->shape = shape;
// 		bd->fixtures.push_back(fixture);
// 	}
//}
//
//bool BodyEditStage::RemoveShape(d2d::Shape* shape)
//{
//	return libshape::StagePanel::RemoveShape(shape);
//
//	 	if (m_sprite && m_sprite->GetUserData())
//	 	{
//	 		libmodeling::Body* bd = static_cast<libmodeling::Body*>(m_sprite->GetUserData());
//	 		for (size_t i = 0, n = bd->fixtures.size(); i < n; ++i)
//	 		{
//	 			if (bd->fixtures[i]->shape == shape)
//	 			{
//	 				delete bd->fixtures[i];
//	 				bd->fixtures.erase(bd->fixtures.begin() + i);
//	 				break;
//	 			}
//	 		}
//	 	}
//}

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
// 			d2d::Shape* shape = bd->fixtures[i]->shape;
// 			shape->Retain();
// 			m_shapes.push_back(shape);
// 		}
// 	}
}

}
