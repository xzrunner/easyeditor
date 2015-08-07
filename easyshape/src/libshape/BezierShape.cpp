 #include "BezierShape.h"
#include "BezierPropertySetting.h"

namespace libshape
{

BezierShape::BezierShape()
{
}

BezierShape::BezierShape(const BezierShape& bezier)
{
	CopyCtrlNodes(bezier.m_control_nodes);
	createCurve();
}

BezierShape::BezierShape(const d2d::Vector points[4])
{
	CopyCtrlNodes(points);
	createCurve();
}

BezierShape::BezierShape(const d2d::Vector& start, const d2d::Vector& end)
{
	m_control_nodes[0] = start;
	m_control_nodes[3] = end;

	d2d::Vector mid = (start + end) * 0.5f;
	d2d::Vector offset = (end - start) * 0.5f;
	m_control_nodes[1] = mid + d2d::Math::rotateVectorRightAngle(offset, true);
	m_control_nodes[2] = mid + d2d::Math::rotateVectorRightAngle(offset, false);

	createCurve();
}

BezierShape::~BezierShape()
{

}

BezierShape* BezierShape::Clone() const
{
	return new BezierShape(*this);
}

bool BezierShape::isContain(const d2d::Vector& pos) const
{
	bool ret = false;
	for (size_t i = 0; i < CTRL_NODE_COUNT; ++i) {
		if (d2d::Math::getDistance(pos, m_control_nodes[i]) < RADIUS) {
			ret = true;
			break;
		}
	}
	return ret;
}

void BezierShape::Translate(const d2d::Vector& offset)
{
	ChainShape::Translate(offset);
	for (int i = 0; i < CTRL_NODE_COUNT; ++i) {
		m_control_nodes[i] += offset;
	}
}

void BezierShape::draw(const d2d::Matrix& mt,
					   const d2d::Colorf& color/* = Colorf(0, 0, 0)*/) const
{
	ChainShape::draw(mt, color);

	for (size_t i = 0; i < CTRL_NODE_COUNT; ++i) {
		d2d::Vector pos = d2d::Math::transVector(m_control_nodes[i], mt);
		d2d::PrimitiveDraw::rect(pos, (float)RADIUS, (float)RADIUS, m_style);
	}
}

d2d::IPropertySetting* BezierShape::createPropertySetting(d2d::EditPanelImpl* stage)
{
	return new BezierPropertySetting(stage, this);
}

void BezierShape::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	d2d::IShape::LoadFromFile(value, dir);

	d2d::Vector points[4];
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
	d2d::IShape::StoreToFile(value, dir);

	for (int i = 0; i < BezierShape::CTRL_NODE_COUNT; ++i) {
		value["points"]["x"][i] = m_control_nodes[i].x;
		value["points"]["y"][i] = m_control_nodes[i].y;
	}
}

void BezierShape::createCurve()
{
	const size_t num = std::max(20, (int)(d2d::Math::getDistance(m_control_nodes[0], m_control_nodes[3]) / 10));
	float dt = 1.0f / (num - 1);
	std::vector<d2d::Vector> vertices(num);
	for (size_t i = 0; i < num; ++i)
		vertices[i] = pointOnCubicBezier(i * dt);
	Load(vertices);
}

void BezierShape::Mirror(bool x, bool y)
{
	float cx = getRect().xCenter();
	float cy = getRect().yCenter();
	for (int i = 0; i < CTRL_NODE_COUNT; ++i) {
		if (x) {
			m_control_nodes[i].x = cx * 2 - m_control_nodes[i].x;
		}
		if (y) {
			m_control_nodes[i].y = cy * 2 - m_control_nodes[i].y;			
		}
	}
}

void BezierShape::MoveCtrlNode(const d2d::Vector& from, const d2d::Vector& to)
{
	for (int i = 0; i < CTRL_NODE_COUNT; ++i) {
		if (m_control_nodes[i] == from) {
			m_control_nodes[i] = to;
			createCurve();
		}
	}
}

void BezierShape::CopyCtrlNodes(const d2d::Vector ctrl_points[])
{
	for (int i = 0; i < CTRL_NODE_COUNT; ++i) {
		m_control_nodes[i] = ctrl_points[i];
	}
}

d2d::Vector BezierShape::pointOnCubicBezier(float t)
{
	float ax, bx, cx;
	float ay, by, cy;
	float tSquared, tCubed;
	d2d::Vector result;

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