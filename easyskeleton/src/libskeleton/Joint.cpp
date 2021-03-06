#include "Joint.h"

#include <ee/Sprite.h>

#include <SM_Calc.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Shader.h>
#include <sprite2/RVG.h>
#include <painting2/Color.h>
#include <sprite2/RenderParams.h>
#include <sprite2/Sprite.h>

namespace libskeleton
{

Joint::Joint(const s2::SprPtr& spr, const s2::JointPose& joint_pose)
	: s2::Joint(spr, joint_pose)
{
}

void Joint::DrawSkeleton(const s2::RenderParams& params, bool selected) const
{
	if (selected) {
		s2::RVG::SetColor(pt2::Color(204, 51, 51, 128));
		s2::RVG::Circle(nullptr, params.mt * m_world_pose.trans, RADIUS, true);
	} else {
		s2::RVG::SetColor(pt2::Color(51, 204, 51, 128));
		s2::RVG::Circle(nullptr, params.mt * m_world_pose.trans, RADIUS, true);
		s2::RVG::SetColor(pt2::Color(204, 51, 51, 128));
		s2::RVG::Circle(nullptr, params.mt * m_world_pose.trans, RADIUS, false);
	}

	if (m_parent.lock() && m_skin.spr)
	{
		sm::vec2 s = params.mt * m_world_pose.trans;
		sm::vec2 e = params.mt * m_skin.spr->GetCenter() * 2 - s;

		const float w = 0.1f;
		sm::vec2 mid = s + (e-s)*w;
		sm::vec2 left = mid + sm::rotate_vector_right_angle(s - mid, false);
		sm::vec2 right = mid + sm::rotate_vector_right_angle(s - mid, true);

		if (!selected) {
			s2::RVG::SetColor(pt2::Color(204, 51, 51, 128));
			s2::RVG::Line(nullptr, s, left);
			s2::RVG::Line(nullptr, left, e);
			s2::RVG::Line(nullptr, e, right);
			s2::RVG::Line(nullptr, right, s);
		}

		if (selected) {
			s2::RVG::SetColor(pt2::Color(204, 51, 51, 128));
		} else {
			s2::RVG::SetColor(pt2::Color(204, 204, 51, 128));
		}
		CU_VEC<sm::vec2> face;
		face.push_back(s);
		face.push_back(left);
		face.push_back(right);
		face.push_back(e);
		s2::RVG::TriangleStrip(nullptr, face);
	}
	else
	{
		s2::RVG::Cross(nullptr, params.mt * m_world_pose.trans, 50, 50);
	}

	// fix me
	sl::Shader* shader = sl::ShaderMgr::Instance()->GetShader();
	if (shader) {
		shader->Commit();
	}
}

}