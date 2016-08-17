#include "BodyEditStage.h"

namespace emodeling
{

BodyEditStage::BodyEditStage(wxWindow* parent, wxTopLevelWindow* frame, 
							 eshape::Symbol* sym)
	: eshape::StagePanel(parent, frame, sym)
{
}

//bool BodyEditStage::InsertShape(ee::Shape* shape)
//{
//	return eshape::StagePanel::InsertShape(shape);
//
// 	if (m_spr && m_spr->GetUserData())
// 	{
// 		Body* bd = static_cast<Body*>(m_spr->GetUserData());
// 		Fixture* fixture = new Fixture;
// 		fixture->body = bd;
// 		shape->AddReference();
// 		fixture->shape = shape;
// 		bd->fixtures.push_back(fixture);
// 	}
//}
//
//bool BodyEditStage::RemoveShape(ee::Shape* shape)
//{
//	return eshape::StagePanel::RemoveShape(shape);
//
//	 	if (m_spr && m_spr->GetUserData())
//	 	{
//	 		Body* bd = static_cast<Body*>(m_spr->GetUserData());
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
// 	if (m_spr && m_spr->GetUserData())
// 	{
// 		Body* bd = static_cast<Body*>(m_spr->GetUserData());
// 		m_shapes.reserve(bd->fixtures.size());
// 		for (size_t i = 0, n = bd->fixtures.size(); i < n; ++i)
// 		{
// 			ee::Shape* shape = bd->fixtures[i]->shape;
// 			shape->AddReference();
// 			m_shapes.push_back(shape);
// 		}
// 	}
}

}
