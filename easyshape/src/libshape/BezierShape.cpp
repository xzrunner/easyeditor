#include "BezierShape.h"
#include "BezierPropertySetting.h"

#include <ee/Math2D.h>
#include <ee/EE_RVG.h>

namespace eshape
{

BezierShape::BezierShape()
{
	m_draw_dir = false;
}

BezierShape::BezierShape(const BezierShape& bezier)
{
	m_draw_dir = false;

	CopyCtrlNodes(bezier.m_control_nodes);
	createCurve();
}

BezierShape::BezierShape(const ee::Vector points[4])
{
	m_draw_dir = false;

	CopyCtrlNodes(points);
	createCurve();
}

BezierShape::BezierShape(const ee::Vector& start, const ee::Vector& end)
{
	m_draw_dir = false;

	m_control_nodes[0] = start;
	m_control_nodes[3] = end;

	ee::Vector mid = (start + end) * 0.5f;
	ee::Vector offset = (end - start) * 0.5f;
	m_control_nodes[1] = mid + ee::Math2D::RotateVectorRightAngle(offset, true);
	m_control_nodes[2] = mid + ee::Math2D::RotateVectorRightAngle(offset, false);

	createCurve();
}

BezierShape::~BezierShape()
{

}

BezierShape* BezierShape::Clone() const
{
	return new BezierShape(*this);
}

bool BezierShape::IsContain(const ee::Vector& pos) const
{
	bool ret = false;
	for (size_t i = 0; i < CTRL_NODE_COUNT; ++i) {
		if (ee::Math2D::GetDistance(pos, m_control_nodes[i]) < RADIUS) {
			ret = true;
			break;
		}
	}
	return ret;
}

void BezierShape::Translate(const ee::Vector& offset)
{
	ChainShape::Translate(offset);
	for (int i = 0; i < CTRL_NODE_COUNT; ++i) {
		m_control_nodes[i] += offset;
	}
}

void BezierShape::Draw(const ee::Matrix& mt, const ee::ColorTrans& color) const
{
	ChainShape::Draw(mt, color);

	for (size_t i = 0; i < CTRL_NODE_COUNT; ++i) {
		ee::Vector pos = ee::Math2D::TransVector(m_control_nodes[i], mt);
		ee::RVG::Rect(pos, (float)RADIUS, (float)RADIUS, false);
	}
}

ee::PropertySetting* BezierShape::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new BezierPropertySetting(stage, this);
}

void BezierShape::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	ee::Shape::LoadFromFile(value, dir);

	ee::Vector points[4];
	for (size_t i = 0; i < 4; ++i)
	{
		points[i].x = value["points"]["x"][i].asDouble();
		points[i].y = value["points"]["y"][i].asDouble();
	}

	CopyCtrlNodes(points);
	createCurve();
}

void BezierShape::StoreToFile(Json::Value& value, const std::string& dir) const
{
	ee::Shape::StoreToFile(value, dir);

	for (int i = 0; i < BezierShape::CTRL_NODE_COUNT; ++i) {
		value["points"]["x"][i] = m_control_nodes[i].x;
		value["points"]["y"][i] = m_control_nodes[i].y;
	}
}

void BezierShape::createCurve()
{
	const size_t num = std::max(20, (int)(ee::Math2D::GetDistance(m_control_nodes[0], m_control_nodes[3]) / 10));
	float dt = 1.0f / (num - 1);
	std::vector<ee::Vector> vertices(num);
	for (size_t i = 0; i < num; ++i)
		vertices[i] = pointOnCubicBezier(i * dt);
	Load(vertices);
}

void BezierShape::Mirror(bool x, bool y)
{
	float cx = GetRect().CenterX();
	float cy = GetRect().CenterY();
	for (int i = 0; i < CTRL_NODE_COUNT; ++i) {
		if (x) {
			m_control_nodes[i].x = cx * 2 - m_control_nodes[i].x;
		}
		if (y) {
			m_control_nodes[i].y = cy * 2 - m_control_nodes[i].y;			
		}
	}
}

void BezierShape::MoveCtrlNode(const ee::Vector& from, const ee::Vector& to)
{
	for (int i = 0; i < CTRL_NODE_COUNT; ++i) {
		if (m_control_nodes[i] == from) {
			m_control_nodes[i] = to;
			createCurve();
		}
	}
}

void BezierShape::CopyCtrlNodes(const ee::Vector ctrl_points[])
{
	for (int i = 0; i < CTRL_NODE_COUNT; ++i) {
		m_control_nodes[i] = ctrl_points[i];
	}
}

ee::Vector BezierShape::pointOnCubicBezier(float t)
{
	float ax, bx, cx;
	float ay, by, cy;
	float tSquared, tCubed;
	ee::Vector result;

	cx = 3.0f * (m_control_nodes[1].x - m_control_nodes[0].x);
	bx = 3.0f * (m_control_nodes[2].x - m_control_nodes[1].x) - cx;
	ax = m_control_nodes[3].x - m_control_nodes[0].x - cx - bx;

	cy = 3.0f * (m_control_nodes[1].y - m_control_nodes[0].y);
	by = 3.0f * (m_control_nodes[2].y - m_control_nodes[1].y) - cy;
	ay = m_control_nodes[3].y - m_control_nodes[0].y - cy - by;

	tSquared = t * t;
	tCubed = tSquared * t;

	result.x = (ax * tCubed) + (bx * tSquared) + (cx * t) + m_control_nodes[0].x;
	result.y = (ay * tCubed) + (by * tSquared) + (cy * t) + m_control_nodes[0].y;

	return result;
}

}