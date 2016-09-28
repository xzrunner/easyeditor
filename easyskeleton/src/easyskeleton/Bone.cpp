#include "Bone.h"
#include "Joint.h"

#include <ee/Sprite.h>
#include <ee/Symbol.h>
#include <ee/SpriteRenderer.h>

#include <SM_Calc.h>

namespace eskeleton
{

Bone::Bone()
	: m_skin(NULL)
{
}

Bone::Bone(ee::Sprite* skin)
	: m_skin(skin)
{
	if (m_skin) {
		m_skin->AddReference();
		sm::rect r = skin->GetSymbol()->GetBounding(m_skin);
		m_joints.push_back(new Joint(m_skin, -sm::vec2(r.xmin, r.ymin)));
		m_joints.push_back(new Joint(m_skin, -sm::vec2(r.xmax, r.ymax)));
	}
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

void Bone::Update()
{
	if (!m_skin) {
		return;
	}

	for (int i = 0, n = m_joints.size(); i < n; ++i) {
		m_joints[i]->UpdateToSkin();		
	}
}

void Bone::Draw() const
{
	if (m_skin) {
		ee::SpriteRenderer::Instance()->Draw(m_skin);
	}
	for (int i = 0, n = m_joints.size(); i < n; ++i) {
		m_joints[i]->Draw();
	}
}

Joint* Bone::QueryJoint(const sm::vec2& pos) const
{
	for (int i = 0, n = m_joints.size(); i < n; ++i) {
		Joint* joint = m_joints[i];
		if (sm::dis_pos_to_pos(pos, joint->GetWorldPos()) < Joint::RADIUS) {
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
			if (sm::dis_pos_to_pos(src->GetWorldPos(), dst->GetWorldPos()) < Joint::RADIUS * 2) 
			{
				src->SetWorldPos(dst->GetWorldPos(), true);
				Update();
				dst->Connect(src);
				return true;
			}
		}
		if (const Joint* parent = src->GetParent()) {
			if (sm::dis_pos_to_pos(src->GetWorldPos(), src->GetParent()->GetWorldPos()) > Joint::RADIUS * 2) {
				src->Deconnect();
			}
		}
	}
	return false;
}

void Bone::Translate(const sm::vec2& offset)
{
	m_skin->Translate(offset);
	for (int i = 0, n = m_joints.size(); i < n; ++i) {
		m_joints[i]->Translate(offset);
	}
}

bool Bone::Rotate(float angle)
{
	Joint* base = NULL;
	for (int i = 0, n = m_joints.size(); i < n; ++i) {
		Joint* joint = m_joints[i];
		if (joint->GetParent()) {
			base = joint;
		}
	}

	if (!base) {
		return false;
	}

	base->Rotate(angle);

	m_skin->SetOffset(sm::rotate_vector(base->GetWorldPos() - m_skin->GetCenter(), -m_skin->GetAngle()));
	m_skin->Rotate(angle);

	for (int i = 0, n = m_joints.size(); i < n; ++i) {
		if (m_joints[i] != base) {
			m_joints[i]->UpdateToSkin();
		}
	}

	return true;
}

void Bone::SetSkin(ee::Sprite* skin)
{
	cu::RefCountObjAssign(m_skin, skin);
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

}