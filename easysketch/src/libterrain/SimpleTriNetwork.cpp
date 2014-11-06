#include "SimpleTriNetwork.h"

namespace libterrain
{

static const float EDGE = 10;

static const int MAX_LEVEL = 10;

SimpleTriNetwork::SimpleTriNetwork(const e3d::Camera& cam)
	: m_cam(cam)
{
}

void SimpleTriNetwork::Draw() const
{
	const float HALF_EDGE = EDGE * 0.5f;
	vec3 v0(-HALF_EDGE,  HALF_EDGE, 0), 
		 v1( HALF_EDGE,  HALF_EDGE, 0),
		 v2( HALF_EDGE, -HALF_EDGE, 0),
		 v3(-HALF_EDGE, -HALF_EDGE, 0);
	DrawTriByLevel(v1, v0, v2, 0);
	DrawTriByLevel(v3, v0, v2, 0);
}

void SimpleTriNetwork::DrawTriByLevel(const vec3& v0, const vec3& v1, const vec3& v2, int level) const
{
	vec3 c = (v0 + v1 + v2) / 3;
	float dis = e3d::Math3::GetDistance(c, m_cam.GetPosition());
//	e3d::LogViewer::Instance()->Add(("dis:"+wxString::FromDouble(dis)).ToStdString());

	if (level < MAX_LEVEL && dis < (MAX_LEVEL - level))
	{
		vec3 new_node = (v1 + v2) * 0.5f;
		DrawTriByLevel(new_node, v0, v1, level + 1);
		DrawTriByLevel(new_node, v0, v2, level + 1);
		return;
	}

	e3d::DrawTriLine(v0, v1, v2);
}

}