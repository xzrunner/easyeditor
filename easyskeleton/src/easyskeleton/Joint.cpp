#include "Joint.h"
#include "Bone.h"

#include <ee/Sprite.h>

#include <SM_Calc.h>
#include <shaderlab.h>
#include <sprite2/S2_RVG.h>
#include <sprite2/JointMath.h>

#include <algorithm>

namespace eskeleton
{

const float Joint::RADIUS = 10;

Joint::Joint(ee::Sprite* spr, const sm::vec2& offset)
	: m_parent(NULL)
	, m_skin(spr, -offset)
{
	s2::JointPose src_world(m_skin.spr->GetCenter(), m_skin.spr->GetAngle()),
		      local(-m_skin.pose.trans, -m_skin.pose.rot);
	m_world_pose = s2::JointMath::Local2World(src_world, local);
}

Joint::~Joint()
{
	if (m_parent) {
		m_parent->RemoveReference();
	}
	for_each(m_children.begin(), m_children.end(), cu::RemoveRefFonctor<Joint>());
	if (m_skin.spr) {
		m_skin.spr->RemoveReference();
	}
}

void Joint::Draw() const
{
	s2::RVG::SetColor(s2::Color(51, 204, 51, 128));
	s2::RVG::Circle(m_world_pose.trans, RADIUS, true);
	s2::RVG::SetColor(s2::Color(204, 51, 51, 128));
	s2::RVG::Circle(m_world_pose.trans, RADIUS, false);

	if (m_parent)
	{
		sm::vec2 s = GetWorldPos();
		sm::vec2 e = m_skin.spr->GetCenter() * 2 - s;

		const float w = 0.1f;
		sm::vec2 mid = s + (e-s)*w;
		sm::vec2 left = mid + sm::rotate_vector_right_angle(s - mid, false);
		sm::vec2 right = mid + sm::rotate_vector_right_angle(s - mid, true);

		s2::RVG::SetColor(s2::Color(204, 51, 51, 128));
		s2::RVG::Line(s, left);
		s2::RVG::Line(left, e);
		s2::RVG::Line(e, right);
		s2::RVG::Line(right, s);

		s2::RVG::SetColor(s2::Color(204, 204, 51, 128));
		std::vector<sm::vec2> face;
		face.push_back(s);
		face.push_back(left);
		face.push_back(right);
		face.push_back(e);
		s2::RVG::TriangleStrip(face);
	}

	sl::Shader* shader = sl::ShaderMgr::Instance()->GetShader();
	if (shader) {
		shader->Commit();
	}
}

void Joint::UpdateToJoint()
{
	m_world_pose = s2::JointMath::Local2World(m_parent->m_world_pose, m_local_pose);
	m_skin.Update(this);
	UpdateChildren();
}

void Joint::UpdateToSkin()
{
	s2::JointPose src_world(m_skin.spr->GetCenter(), m_skin.spr->GetAngle()),
		      local(-m_skin.pose.trans, -m_skin.pose.rot);
	m_world_pose = s2::JointMath::Local2World(src_world, local);
	UpdateChildren();
}

void Joint::Translate(const sm::vec2& offset)
{
	m_world_pose.trans += offset;
	for (int i = 0, n = m_children.size(); i < n; ++i) 
	{
		Joint* c = m_children[i];
		Bone* cbone = (Bone*)(c->m_skin.spr->GetUserData());
		cbone->Translate(offset);
	}
}

void Joint::Rotate(float angle)
{
	m_local_pose.rot += angle;
}

void Joint::SetWorldPos(const sm::vec2& pos, bool static_skin)
{
	if (static_skin) {
		m_skin.spr->Translate(pos - m_world_pose.trans);
		m_world_pose.trans = pos;
	} else {
		m_world_pose.trans = pos;

		s2::JointPose src_world(m_skin.spr->GetCenter(), m_skin.spr->GetAngle());
		m_skin.pose.trans = -s2::JointMath::World2Local(src_world, pos);
		m_skin.pose.rot = 0;
	}
}

bool Joint::Connect(Joint* child)
{
	for (int i = 0, n = child->m_children.size(); i < n; ++i) {
		if (child->m_children[i] == this) {
			return false;
		}
	}
	for (int i = 0, n = m_children.size(); i < n; ++i) {
		if (m_children[i] == child) {
			return false;
		}
	}

	child->AddReference();
	m_children.push_back(child);

	cu::RefCountObjAssign(child->m_parent, (Joint*)this);

	return true;
}

void Joint::Deconnect()
{
	if (!m_parent) {
		return;
	}

	for (int i = 0, n = m_parent->m_children.size(); i < n; ++i) {
		if (m_parent->m_children[i] == this) {
			this->RemoveReference();
			m_parent->m_children.erase(m_parent->m_children.begin() + i);
			break;
		}
	}

	m_parent->RemoveReference();
	m_parent = NULL;
}

void Joint::Clear()
{
	Deconnect();
	while (!m_children.empty()) {
		m_children[0]->Deconnect();
	}
	if (m_skin.spr) {
		m_skin.spr->RemoveReference();
		m_skin.spr = NULL;
	}
}

void Joint::UpdateChildren()
{
	for (int i = 0, n = m_children.size(); i < n; ++i) 
	{
		Joint* c = m_children[i];
		c->UpdateToJoint();
		Bone* cbone = (Bone*)(c->m_skin.spr->GetUserData());
		cbone->Update();
	}
}

/************************************************************************/
/* class Joint::Skin                                                    */
/************************************************************************/

Joint::Skin::Skin(ee::Sprite* spr, const sm::vec2& pos)
	: spr(spr)
	, pose(pos, 0)
{
	if (this->spr) {
		this->spr->AddReference();
	}
}

Joint::Skin::~Skin()
{
	if (spr) {
		spr->RemoveReference();
	}
}

void Joint::Skin::Update(const Joint* joint)
{
	s2::JointPose dst = s2::JointMath::Local2World(joint->m_world_pose, pose);
	spr->SetAngle(dst.rot);
	spr->Translate(dst.trans - spr->GetCenter());
}

}