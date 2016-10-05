#include "Bone.h"
#include "Joint.h"

#include <ee/Sprite.h>
#include <ee/Symbol.h>
#include <ee/SpriteRenderer.h>

#include <SM_Calc.h>
#include <sprite2/BoundingBox.h>

namespace eskeleton
{

Bone::Bone()
	: m_skin(NULL)
	, m_is_updating(false)
{
}

Bone::Bone(ee::Sprite* skin)
	: m_skin(skin)
	, m_is_updating(false)
{
	if (m_skin) {
		m_skin->AddReference();
		m_skin->SetObserver(this);
	}
	InitJoints();
}

Bone::~Bone()
{
	if (m_skin) {
		m_skin->RemoveReference();
	}
	for (int i = 0, n = m_joints.size(); i < n; ++i) {
		m_joints[i]->RemoveReference();
	}
}

void Bone::OnSetPosition(ee::Sprite* spr, const sm::vec2& pos)
{
//	Update();
}

void Bone::OnSetAngle(ee::Sprite* spr, float angle)
{
//	Update();
}

void Bone::OnSetScale(ee::Sprite* spr, const sm::vec2& scale)
{
//	Update();
}

void Bone::OnSetShear(ee::Sprite* spr, const sm::vec2& shear)
{
//	Update();
}

void Bone::OnSetOffset(ee::Sprite* spr, const sm::vec2& offset)
{
//	Update();
}

void Bone::Update()
{
	if (m_is_updating) {
		return;
	} else {
		m_is_updating = true;
	}

	if (!m_skin) {
		return;
	}

	for (int i = 0, n = m_joints.size(); i < n; ++i) {
		m_joints[i]->UpdateToSkin();		
	}

	m_is_updating = false;
}

void Bone::Draw() const
{
	if (m_skin) {
		ee::SpriteRenderer::Instance()->Draw(m_skin);
	}
	for (int i = 0, n = m_joints.size(); i < n; ++i) {
		m_joints[i]->DrawSkeleton(s2::RenderParams());
	}
}

Joint* Bone::QueryJoint(const sm::vec2& pos) const
{
	for (int i = 0, n = m_joints.size(); i < n; ++i) {
		Joint* joint = m_joints[i];
		if (sm::dis_pos_to_pos(pos, joint->GetWorldPose().trans) < Joint::RADIUS) {
			return joint;
		}
	}
	return NULL;
}

bool Bone::AutoAbsorb(const Bone* bone)
{
	for (int i = 0, n = m_joints.size(); i < n; ++i) {
		Joint* src = m_joints[i];
		for (int j = 0, m = bone->m_joints.size(); j < m; ++j) {
			Joint* dst = bone->m_joints[j];
			if (sm::dis_pos_to_pos(src->GetWorldPose().trans, dst->GetWorldPose().trans) < Joint::RADIUS * 2) 
			{
				if (dst->ConnectChild(src)) {
					src->BindSkin(dst->GetWorldPose().trans, true);
					Update();
					return true;
				}
			}
		}
		if (const s2::Joint* parent = src->GetParent()) {
			if (sm::dis_pos_to_pos(src->GetWorldPose().trans, src->GetParent()->GetWorldPose().trans) > Joint::RADIUS * 2) {
				src->DeconnectParent();
			}
		}
	}
	return false;
}

void Bone::Translate(const sm::vec2& trans)
{
	m_skin->Translate(trans);
	for (int i = 0, n = m_joints.size(); i < n; ++i) {
		m_joints[i]->Translate(trans);
	}
}

void Bone::Rotate(float rot)
{
	Joint* base = GetCenterJoint();
	if (base) {
		m_skin->SetOffset(sm::rotate_vector(base->GetWorldPose().trans - m_skin->GetCenter(), -m_skin->GetAngle()));
		base->Rotate(rot);
	}

	m_skin->Rotate(rot);

	for (int i = 0, n = m_joints.size(); i < n; ++i) {
		if (m_joints[i] != base) {
			m_joints[i]->UpdateToSkin();
		}
	}
}

void Bone::Scale(const sm::vec2& scale)
{
	Joint* base = GetCenterJoint();
	if (base) {
		m_skin->SetOffset(sm::rotate_vector(base->GetWorldPose().trans - m_skin->GetCenter(), -m_skin->GetAngle()));
		base->Scale(scale);
	}

	m_skin->Scale(scale);

	for (int i = 0, n = m_joints.size(); i < n; ++i) {
		if (m_joints[i] != base) {
			m_joints[i]->UpdateToSkin();
		}
	}
}

void Bone::SetSkin(ee::Sprite* skin)
{
	if (m_skin == skin) {
		return;
	}
	if (m_skin) {
		m_skin->RemoveReference();
		m_skin->SetObserver(NULL);
	}
	m_skin = skin;
	if (m_skin) {
		m_skin->SetObserver(this);
	}
}

void Bone::AddJoint(Joint* joint)
{
	joint->AddReference();
	m_joints.push_back(joint);
}

void Bone::RemoveJoint(Joint* joint)
{
	for (int i = 0, n = m_joints.size(); i < n; ++i) {
		if (m_joints[i] == joint) {
			m_joints.erase(m_joints.begin() + i);
			break;
		}
	}
	joint->Clear();
	joint->RemoveReference();
}

void Bone::InitJoints()
{
	if (!m_skin) {
		return;
	}

	sm::rect r = m_skin->GetBounding()->GetSize();
	sm::vec2 center = m_skin->GetCenter();
	sm::vec2 min(r.xmin, r.ymin), max(r.xmax, r.ymax);
	m_joints.push_back(new Joint(m_skin, s2::JointPose(min - center, 0)));
	m_joints.push_back(new Joint(m_skin, s2::JointPose(max - center, 0)));
}

Joint* Bone::GetCenterJoint()
{
	Joint* base = NULL;
	for (int i = 0, n = m_joints.size(); i < n; ++i) {
		Joint* joint = m_joints[i];
		if (joint->GetParent()) {
			base = joint;
		}
	}
	return base;
}

}