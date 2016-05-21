//#include "BezierShape.h"
//#include "BezierPropertySetting.h"
//
//#include <ee/Math2D.h>
//#include <ee/EE_RVG.h>
//
//namespace eshape
//{
//
//// BezierShape::BezierShape(const sm::vec2 points[4])
//// {
//// 	m_draw_dir = false;
//// 
//// 	CopyCtrlNodes(points);
//// 	createCurve();
//// }
//
//BezierShape::BezierShape(const sm::vec2& start, const sm::vec2& end)
//	: m_core(start, end)
//{
//}
//
//BezierShape* BezierShape::Clone() const
//{
//	return new BezierShape(*this);
//}
//
//void BezierShape::Translate(const sm::vec2& offset)
//{
//	sm::vec2 ctrl_nodes[s2::BezierShape::CTRL_NODE_COUNT];
//	memcpy(ctrl_nodes, m_core.GetCtrlNodes(), sizeof(ctrl_nodes));
//	for (int i = 0; i < s2::BezierShape::CTRL_NODE_COUNT; ++i) {
//		ctrl_nodes[i] += offset;
//	}
//	m_core.SetCtrlNodes(ctrl_nodes);
//}
//
//ee::PropertySetting* BezierShape::CreatePropertySetting(ee::EditPanelImpl* stage)
//{
//	return new BezierPropertySetting(stage, this);
//}
//
//void BezierShape::LoadFromFile(const Json::Value& value, const std::string& dir)
//{
//	ee::Shape::LoadFromFile(value, dir);
//
//	sm::vec2 points[4];
//	for (size_t i = 0; i < 4; ++i)
//	{
//		points[i].x = value["points"]["x"][i].asDouble();
//		points[i].y = value["points"]["y"][i].asDouble();
//	}
//
//	m_core.SetCtrlNodes(points);
//}
//
//void BezierShape::StoreToFile(Json::Value& value, const std::string& dir) const
//{
//	ee::Shape::StoreToFile(value, dir);
//
//	const sm::vec2* ctrl_nodes = m_core.GetCtrlNodes();
//	for (int i = 0; i < s2::BezierShape::CTRL_NODE_COUNT; ++i) {
//		value["points"]["x"][i] = ctrl_nodes[i].x;
//		value["points"]["y"][i] = ctrl_nodes[i].y;
//	}
//}
//
//void BezierShape::Mirror(bool x, bool y)
//{
//	sm::vec2 ctrl_nodes[s2::BezierShape::CTRL_NODE_COUNT];
//	memcpy(ctrl_nodes, m_core.GetCtrlNodes(), sizeof(ctrl_nodes));
//
//	sm::vec2 center = GetRect().Center();
//	for (int i = 0; i < s2::BezierShape::CTRL_NODE_COUNT; ++i) 
//	{
//		if (x) {
//			ctrl_nodes[i].x = center.x * 2 - ctrl_nodes[i].x;
//		}
//		if (y) {
//			ctrl_nodes[i].y = center.y * 2 - ctrl_nodes[i].y;			
//		}
//	}
//	m_core.SetCtrlNodes(ctrl_nodes);
//}
//
//void BezierShape::MoveCtrlNode(const sm::vec2& from, const sm::vec2& to)
//{
//	sm::vec2 ctrl_nodes[s2::BezierShape::CTRL_NODE_COUNT];
//	memcpy(ctrl_nodes, m_core.GetCtrlNodes(), sizeof(ctrl_nodes));
//	for (int i = 0; i < s2::BezierShape::CTRL_NODE_COUNT; ++i) {
//		if (ctrl_nodes[i] == from) {
//			ctrl_nodes[i] = to;
//		}
//	}
//	m_core.SetCtrlNodes(ctrl_nodes);
//}
//
//}