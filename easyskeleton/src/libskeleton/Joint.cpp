#include "Joint.h"

#include <ee/Sprite.h>

#include <SM_Calc.h>
#include <shaderlab.h>
#include <sprite2/S2_RVG.h>
#include <sprite2/Color.h>
#include <sprite2/RenderParams.h>
#include <sprite2/S2_Sprite.h>
#include <sprite2/BoundingBox.h>

namespace libskeleton
{

Joint::Joint(s2::Sprite* spr, const s2::LocalPose& joint_pose)
	: s2::Joint(spr, joint_pose)
{
}

void Joint::DrawSkeleton(const s2::RenderParams& params, bool selected) const
{
	if (selected) {
		s2::RVG::SetColor(s2::Color(204, 51, 51, 128));
		s2::RVG::Circle(params.mt * m_world.pos, RADIUS, true);
	} else {
		s2::RVG::SetColor(s2::Color(51, 204, 51, 128));
		s2::RVG::Circle(params.mt * m_world.pos, RADIUS, true);
		s2::RVG::SetColor(s2::Color(204, 51, 51, 128));
		s2::RVG::Circle(params.mt * m_world.pos, RADIUS, false);
	}

	if (m_parent)
	{
		sm::vec2 s = params.mt * m_world.pos;
		sm::vec2 e = params.mt * m_skin.spr->GetCenter() * 2 - s;

		const float w = 0.1f;
		sm::vec2 mid = s + (e-s)*w;
		sm::vec2 left = mid + sm::rotate_vector_right_angle(s - mid, false);
		sm::vec2 right = mid + sm::rotate_vector_right_angle(s - mid, true);

		if (!selected) {
			s2::RVG::SetColor(s2::Color(204, 51, 51, 128));
			s2::RVG::Line(s, left);
			s2::RVG::Line(left, e);
			s2::RVG::Line(e, right);
			s2::RVG::Line(right, s);
		}

		if (selected) {
			s2::RVG::SetColor(s2::Color(204, 51, 51, 128));
		} else {
			s2::RVG::SetColor(s2::Color(204, 204, 51, 128));
		}
		std::vector<sm::vec2> face;
		face.push_back(s);
		face.push_back(left);
		face.push_back(right);
		face.push_back(e);
		s2::RVG::TriangleStrip(face);
	}
	else
	{
		s2::RVG::Cross(params.mt * m_skin.spr->GetCenter(), 50, 50);
	}

	std::vector<sm::vec2> bounding;
	m_skin.spr->GetBounding()->GetBoundPos(bounding);
	s2::RVG::Polyline(bounding, true);

	// fix me
	sl::Shader* shader = sl::ShaderMgr::Instance()->GetShader();
	if (shader) {
		shader->Commit();
	}
}

}