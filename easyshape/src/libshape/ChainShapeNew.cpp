//#include "ChainShape.h"
//#include "ChainPropertySetting.h"
//
//#include <ee/Math2D.h>
//#include <ee/EE_RVG.h>
//#include <ee/SettingData.h>
//
//namespace eshape
//{
//
//ChainShape::ChainShape()
//{
//	m_draw_dir = false;
//}
//
//ChainShape::ChainShape(const std::vector<sm::vec2>& vertices, bool closed)
//	: m_core(vertices, closed)
//{
//	m_draw_dir = !closed;
//}
//
//ChainShape* ChainShape::Clone() const
//{
//	return new ChainShape(*this);
//}
//
//void ChainShape::Translate(const sm::vec2& offset)
//{
//	std::vector<sm::vec2> vertices = m_core.GetVertices();
//	for (int i = 0, n = vertices.size(); i < n; ++i) {
//		vertices[i] += offset;
//	}
//	m_core.SetVertices(vertices);
//}
//
//void ChainShape::Draw(const sm::mat4& mt, const s2::RenderColor& color) const
//{
//	if (ee::SettingData::ctl_pos_sz != 0) {
//		float len = ee::Math2D::TransLen(ee::SettingData::ctl_pos_sz, mt);
//		ee::RVG::Color(s2::Color(102, 204, 102));
//		ee::RVG::Circles(m_core.GetVertices(), len, true);
//	}
//	if (m_draw_dir) {
//		ee::RVG::Color(color.mul);
//		ee::RVG::Circle(m_core.GetVertices()[0], 10, true);
//	}
//}
//
//ee::PropertySetting* ChainShape::CreatePropertySetting(ee::EditPanelImpl* stage)
//{
//	return new ChainPropertySetting(stage, this);
//}
//
//void ChainShape::LoadFromFile(const Json::Value& value, const std::string& dir)
//{
//	ee::Shape::LoadFromFile(value, dir);
//
//	std::vector<sm::vec2> vertices;
//	int num = value["vertices"]["x"].size();
//	vertices.resize(num);
//	for (int i = 0; i < num; ++i) {
//		vertices[i].x = value["vertices"]["x"][i].asDouble();
//		vertices[i].y = value["vertices"]["y"][i].asDouble();
//	}
//	m_core.SetVertices(vertices);
//
//	bool closed = value["closed"].asBool();
//	m_core.SetClosed(closed);
//
//	m_draw_dir = !closed;
//}
//
//void ChainShape::StoreToFile(Json::Value& value, const std::string& dir) const
//{
//	ee::Shape::StoreToFile(value, dir);
//
//	const std::vector<sm::vec2>& vertices = m_core.GetVertices();
//	for (int i = 0, n = vertices.size(); i < n; ++i) {
//		value["vertices"]["x"][i] = vertices[i].x;
//		value["vertices"]["y"][i] = vertices[i].y;
//	}
//	value["closed"] = IsClosed();
//}
//
//void ChainShape::Add(size_t index, const sm::vec2& pos)
//{
//	std::vector<sm::vec2> vertices = m_core.GetVertices();
//	if (index <= vertices.size()) {
//		vertices.insert(vertices.begin() + index, pos);
//	}
//	m_core.SetVertices(vertices);
//}
//
//void ChainShape::Remove(const sm::vec2& pos)
//{
//	int idx = -1;
//	const std::vector<sm::vec2>& vertices = m_core.GetVertices();
//	std::vector<sm::vec2>::const_iterator itr = vertices.begin();
//	for (int i = 0; itr != vertices.end(); ++itr, ++i) {
//		if (*itr == pos) 
//		{
//			idx = i;
//			break;
//		}
//	}
//	if (idx == -1) {
//		return;
//	}
//
//	std::vector<sm::vec2> vertices_new = m_core.GetVertices();
//	vertices_new.erase(vertices_new.begin() + idx);
//	m_core.SetVertices(vertices_new);
//}
//
//void ChainShape::Change(const sm::vec2& from, const sm::vec2& to)
//{
//	int idx = 0;
//	const std::vector<sm::vec2>& vertices = m_core.GetVertices();
//	for (int n = vertices.size(); idx < n; ++idx) {
//		if (vertices[idx] == from) {
//			break;
//		}
//	}
//	if (idx == vertices.size()) {
//		return;
//	}
//
//	std::vector<sm::vec2> vertices_new = m_core.GetVertices();
//	vertices_new[idx] = to;
//	m_core.SetVertices(vertices_new);
//}
//
//void ChainShape::Load(const std::vector<sm::vec2>& vertices)
//{
//	m_core.SetVertices(vertices);
//}
//
//}