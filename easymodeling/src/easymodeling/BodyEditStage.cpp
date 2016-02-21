#include "BodyEditStage.h"

namespace emodeling
{

BodyEditStage::BodyEditStage(wxWindow* parent, wxTopLevelWindow* frame, 
							 eshape::Symbol* symbol)
	: eshape::StagePanel(parent, frame, symbol)
{
}

//bool BodyEditStage::InsertShape(ee::Shape* shape)
//{
//	return eshape::StagePanel::InsertShape(shape);
//
// 	if (m_sprite && m_sprite->GetUserData())
// 	{
// 		Body* bd = static_cast<Body*>(m_sprite->GetUserData());
// 		Fixture* fixture = new Fixture;
// 		fixture->body = bd;
// 		shape->Retain();
// 		fixture->shape = shape;
// 		bd->fixtures.push_back(fixture);
// 	}
//}
//
//bool BodyEditStage::RemoveShape(ee::Shape* shape)
//{
//	return eshape::StagePanel::RemoveShape(shape);
//
//	 	if (m_sprite && m_sprite->GetUserData())
//	 	{
//	 		Body* bd = static_cast<Body*>(m_sprite->GetUserData());
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
// 	eshape::StagePanel::loadShapes();
// 
// 	if (m_sprite && m_sprite->GetUserData())
// 	{
// 		Body* bd = static_cast<Body*>(m_sprite->GetUserData());
// 		m_shapes.reserve(bd->fixtures.size());
// 		for (size_t i = 0, n = bd->fixtures.size(); i < n; ++i)
// 		{
// 			ee::Shape* shape = bd->fixtures[i]->shape;
// 			shape->Retain();
// 			m_shapes.push_back(shape);
// 		}
// 	}
}

}
