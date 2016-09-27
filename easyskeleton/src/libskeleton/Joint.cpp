#include "Joint.h"
#include "Bone.h"

#include <ee/Sprite.h>

#include <SM_Calc.h>
#include <sprite2/S2_RVG.h>

#include <algorithm>

namespace eskeleton
{

const float Joint::RADIUS = 10;

Joint::Joint(ee::Sprite* spr, const sm::vec2& offset)
	: m_parent(NULL)
	, m_world_angle(0)
	, m_skin(spr, -offset)
{
	m_world_pos = spr->GetCenter() + offset;
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
	s2::RVG::Circle(m_world_pos, RADIUS, true);
	s2::RVG::SetColor(s2::Color(204, 51, 51, 128));
	s2::RVG::Circle(m_world_pos, RADIUS, false);

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
	}
}

void Joint::UpdateToJoint()
{
	CalWorld(m_parent->m_world_pos, m_parent->m_world_angle, m_pose.pos, m_pose.angle, m_world_pos, m_world_angle);
	m_skin.Update(this);
	UpdateChildren();
}

void Joint::UpdateToSkin()
{
	CalWorld(m_skin.spr->GetCenter(), m_skin.spr->GetAngle(), -m_skin.pos, -m_skin.angle, m_world_pos, m_world_angle);
	UpdateChildren();
}

void Joint::Translate(const sm::vec2& offset)
{
	m_world_pos += offset;
	for (int i = 0, n = m_children.size(); i < n; ++i) 
	{
		Joint* c = m_children[i];
		Bone* cbone = (Bone*)(c->m_skin.spr->GetUserData());
		cbone->Translate(offset);
	}
}

void Joint::SetWorldPos(const sm::vec2& pos, bool static_skin)
{
	if (static_skin) {
		m_skin.spr->Translate(pos - m_world_pos);
		m_world_pos = pos;
	} else {
		m_world_pos = pos;
		m_skin.pos = -sm::rotate_vector(m_world_pos - m_skin.spr->GetCenter(), -m_world_angle);
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

void Joint::CalWorld(const sm::vec2& src_world_pos, float src_world_angle, 
					 const sm::vec2& local_pos, float local_angle, 
					 sm::vec2& dst_world_pos, float& dst_world_angle)
{
	dst_world_angle = src_world_angle + local_angle;
	dst_world_pos = src_world_pos + sm::rotate_vector(local_pos, dst_world_angle);
}

/************************************************************************/
/* class Joint::Skin                                                    */
/************************************************************************/

Joint::Skin::Skin(ee::Sprite* spr, const sm::vec2& pos)
	: spr(spr)
	, pos(pos)
	, angle(0)
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
	sm::vec2 s_pos;
	float s_angle;
	Joint::CalWorld(joint->m_world_pos, joint->m_world_angle, pos, angle, s_pos, s_angle);
	spr->SetPosition(s_pos);
	spr->SetAngle(s_angle);
}

}