#include "PackAnim2.h"
#include "PackNodeFactory.h"
#include "binary_io.h"
#include "PackCoords.h"

#include <easyanim2.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <rigging.h>
#include <sprite2/SymType.h>
#include <sprite2/MeshSymbol.h>
#include <sprite2/Skeleton2Mesh.h>
#include <sprite2/TrianglesMesh.h>
#include <simp/NodeAnim2.h>
#include <simp/simp_types.h>

namespace esprpacker
{

PackAnim2::PackAnim2(const libanim2::Symbol* sym)
{
	Init(sym);
}

void PackAnim2::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"anim2\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(GetID()));

	lua::assign_with_end(gen, "joints_num", m_joints.size());
	{
		lua::TableAssign ta(gen, "joints", true);
		for (int i = 0, n = m_joints.size(); i < n; ++i) {
			m_joints[i].PackToLuaString(gen);
		}
	}

	lua::assign_with_end(gen, "skins_num", m_skins.size());
	{
		lua::TableAssign ta(gen, "skins", true);
		for (int i = 0, n = m_skins.size(); i < n; ++i) {
			m_skins[i].PackToLuaString(gen);
		}
	}

	lua::assign_with_end(gen, "slots_num", m_slots.size());
	{
		lua::TableAssign ta(gen, "slots", true);
		for (int i = 0, n = m_slots.size(); i < n; ++i) {
			m_slots[i].PackToLuaString(gen);
		}
	}

	{
		lua::TableAssign ta(gen, "tl_joints", true);
		for (int i = 0, n = m_tl_joints.size(); i < n; ++i) {
			m_tl_joints[i].PackToLuaString(gen);
		}
	}
	{
		lua::TableAssign ta(gen, "tl_skins", true);
		for (int i = 0, n = m_tl_skins.size(); i < n; ++i) {
			m_tl_skins[i].PackToLuaString(gen);
		}
	}
	{
		lua::TableAssign ta(gen, "tl_deforms", true);
		for (int i = 0, n = m_tl_deforms.size(); i < n; ++i) {
			m_tl_deforms[i].PackToLuaString(gen);
		}
	}

	gen.detab();
	gen.line("},");
}

int PackAnim2::SizeOfUnpackFromBin() const
{
	int sz = simp::NodeAnim2::Size();
	// joints
	sz += sizeof(uint16_t);
	sz += simp::NodeAnim2::JointSize() * m_joints.size();
	// skins
	sz += sizeof(uint16_t);
	sz += simp::NodeAnim2::SkinSize() * m_skins.size();
	// slots
	sz += sizeof(uint16_t);
	sz += simp::NodeAnim2::SlotSize() * m_slots.size();
	// tl joints
	sz += sizeof(uint16_t);
	sz += simp::NodeAnim2::TLJointSize() * m_tl_joints.size();
	// tl skins
	sz += sizeof(uint16_t);
	sz += simp::NodeAnim2::TLSkinSize() * m_tl_skins.size();
	// tl deforms
	sz += sizeof(uint16_t);
	sz += simp::NodeAnim2::TLDeformSize() * m_tl_deforms.size();
	return sz;
}

int PackAnim2::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);			// id
	sz += sizeof(uint8_t);			// type
	sz += sizeof(uint16_t);         // root
	// joints
	sz += sizeof(uint16_t);
	for (int i = 0, n = m_joints.size(); i < n; ++i) {
		sz += m_joints[i].SizeOfPackToBin();
	}
	// skins
	sz += sizeof(uint16_t);
	for (int i = 0, n = m_skins.size(); i < n; ++i) {
		sz += m_skins[i].SizeOfPackToBin();
	}
	// slots
	sz += sizeof(uint16_t);
	for (int i = 0, n = m_slots.size(); i < n; ++i) {
		sz += m_slots[i].SizeOfPackToBin();
	}
	// tl joints
	for (int i = 0, n = m_tl_joints.size(); i < n; ++i) {
		sz += m_tl_joints[i].SizeOfPackToBin();
	}
	// tl skins
	for (int i = 0, n = m_tl_skins.size(); i < n; ++i) {
		sz += m_tl_skins[i].SizeOfPackToBin();
	}
	// tl deforms
	for (int i = 0, n = m_tl_deforms.size(); i < n; ++i) {
		sz += m_tl_deforms[i].SizeOfPackToBin();
	}
	return sz;
}

void PackAnim2::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = GetID();
	pack(id, ptr);

	uint8_t type = simp::TYPE_ANIM2;
	pack(type, ptr);

	uint16_t root = m_root;
	pack(root, ptr);

	// joints
	uint16_t joints_n = m_joints.size();
	pack(joints_n, ptr);
	for (int i = 0; i < joints_n; ++i) {
		m_joints[i].PackToBin(ptr);
	}
	// skins
	uint16_t skins_n = m_skins.size();
	pack(skins_n, ptr);
	for (int i = 0; i < skins_n; ++i) {
		m_skins[i].PackToBin(ptr);
	}
	// slots
	uint16_t slots_n = m_slots.size();
	pack(slots_n, ptr);
	for (int i = 0; i < slots_n; ++i) {
		m_slots[i].PackToBin(ptr);
	}
	// tl joints
	for (int i = 0, n = m_tl_joints.size(); i < n; ++i) {
		m_tl_joints[i].PackToBin(ptr);
	}
	// tl skins
	for (int i = 0, n = m_tl_skins.size(); i < n; ++i) {
		m_tl_skins[i].PackToBin(ptr);
	}
	// tl deforms
	for (int i = 0, n = m_tl_deforms.size(); i < n; ++i) {
		m_tl_deforms[i].PackToBin(ptr);
	}
}

void PackAnim2::Init(const libanim2::Symbol* sym)
{
	const rg_animation* anim = sym->GetAnim();
	InitSkeleton(anim->sk);
	InitTimeline(&anim->timeline);
}

void PackAnim2::InitSkeleton(const rg_skeleton* sk)
{
	// root
	m_root = sk->root;

	// joints
	m_joints.reserve(sk->joint_count);
	for (int i = 0; i < sk->joint_count; ++i) 
	{
		rg_joint* src = sk->joints[i];
		Joint dst;
		dst.name = src->name;
		dst.parent = src->parent;
		dst.children_count = src->children_count;
		dst.local.Init(src->local_pose);
		m_joints.push_back(dst);
	}

	// skins
	m_skins.reserve(sk->skin_count);
	for (int i = 0; i < sk->skin_count; ++i)
	{
		const rg_skin& src = sk->skins[i];
		Skin dst;
		s2::Symbol* sym = static_cast<s2::Symbol*>(src.ud);
		dst.node = PackNodeFactory::Instance()->Create(VI_DOWNCASTING<ee::Symbol*>(sym));
		switch (sym->Type())
		{
		case s2::SYM_IMAGE:
			dst.type = SKIN_IMG;
			break;
		case s2::SYM_MESH:
			{
				const s2::Mesh* mesh = dynamic_cast<s2::MeshSymbol*>(sym)->GetMesh();
				if (dynamic_cast<const s2::TrianglesMesh*>(mesh)) {
					dst.type = SKIN_MESH;
				} else if (dynamic_cast<const s2::Skeleton2Mesh*>(mesh)) {
					dst.type = SKIN_JOINT_MESH;
				} else {
					assert(0);
				}
			}
			break;
		}
		dst.local.Init(src.local);
		m_skins.push_back(dst);
	}

	// slots
	m_slots.reserve(sk->slot_count);
	for (int i = 0; i < sk->slot_count; ++i)
	{
		const rg_slot& src = sk->slots[i];
		Slot dst;
		dst.joint = src.joint;
		dst.skin = src.skin;
		m_slots.push_back(dst);
	}
}

void PackAnim2::InitTimeline(const rg_timeline* tl)
{
	// joints
	m_tl_joints.reserve(m_joints.size());
	for (int i = 0, n = m_joints.size(); i < n; ++i)
	{
		rg_tl_joint* src = tl->joints[i];
		TL_Joint dst;
		if (!src) {
			dst.type = 0;
			memset(dst.dims_count, 0, sizeof(dst.dims_count));
		} else {
			dst.type = src->type;

			int count = 0;
			for (int j = 0; j < DIM_COUNT; ++j) {
				count += src->dims_count[j];
				dst.dims_count[j] = src->dims_count[j];
			}

			dst.samples.reserve(count);
			for (int j = 0; j < count; ++j) 
			{
				rg_joint_sample* s_src = &src->samples[j];
				JointSample s_dst;
				s_dst.time = s_src->time;
				s_dst.lerp = s_src->lerp;
				s_dst.data = s_src->data;
				dst.samples.push_back(s_dst);
			}
		}
		m_tl_joints.push_back(dst);
	}

	// skins
	m_tl_skins.reserve(m_slots.size());
	for (int i = 0, n = m_slots.size(); i < n; ++i)
	{
		rg_tl_skin* src = tl->skins[i];
		TL_Skin dst;

		dst.samples.reserve(src->skin_count);
		for (int j = 0; j < src->skin_count; ++j)
		{
			rg_skin_sample* s_src = &src->skins[j];
			SkinSample s_dst;
			s_dst.skin = s_src->skin;
			s_dst.time = s_src->time;
			dst.samples.push_back(s_dst);
		}

		m_tl_skins.push_back(dst);
	}

	// deforms
	m_tl_deforms.reserve(m_skins.size());
	for (int i = 0, n = m_skins.size(); i < n; ++i)
	{
		rg_tl_deform* src = tl->deforms[i];
		TL_Deform dst;

		if (src)
		{
			for (int j = 0; j < src->count; ++j)
			{
				rg_deform_sample* s_src = &src->samples[j];
				DeformSample s_dst;
				s_dst.time = s_src->time;
				s_dst.offset = s_src->offset;
				s_dst.data.reserve(s_src->count);
				int ptr = 0;
				for (int k = 0; k < s_src->count; ++k) {					
					s_dst.data.push_back(sm::vec2(s_src->data[ptr++], s_src->data[ptr++]));
				}
				dst.deforms.push_back(s_dst);
			}
		}

		m_tl_deforms.push_back(dst);
	}
}

/************************************************************************/
/* struct PackAnim2::SRT                                                */
/************************************************************************/

void PackAnim2::Srt::
PackToLuaString(ebuilder::CodeGenerator& gen) const
{
	lua::connect(gen, 5,
		lua::assign("trans_x", trans.x),
		lua::assign("trans_y", trans.y),
		lua::assign("rot", rot),
		lua::assign("scale_x", scale.x),
		lua::assign("scale_y", scale.y));
}

void PackAnim2::Srt::
Init(const rg_pose_srt& srt)
{
	trans.Set(srt.trans[0], srt.trans[1]);
	rot = srt.rot;
	scale.Set(srt.scale[0], srt.scale[1]);
}

int PackAnim2::Srt::
SizeOfPackToBin() const
{
	return sizeof(float) * 5;
}

void PackAnim2::Srt::
PackToBin(uint8_t** ptr) const
{
	float val;

	val = trans.x;
	pack(val, ptr);
	val = trans.y;
	pack(val, ptr);

	val = rot;
	pack(val, ptr);

	val = scale.x;
	pack(val, ptr);
	val = scale.y;
	pack(val, ptr);
}

/************************************************************************/
/* struct PackAnim2::Joint                                              */
/************************************************************************/

void PackAnim2::Joint::
PackToLuaString(ebuilder::CodeGenerator& gen) const
{
	lua::assign_with_end(gen, "name", name);
	lua::connect(gen, 2,
		lua::assign("parent", parent),
		lua::assign("children_count", children_count));
	local.PackToLuaString(gen);
}

int PackAnim2::Joint::
SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint16_t);		// parent
	sz += sizeof(uint16_t);		// children_count
	sz += local.SizeOfPackToBin();
	return sz;
}

void PackAnim2::Joint::
PackToBin(uint8_t** ptr) const
{
	uint16_t _parent = parent;
	pack(_parent, ptr);

	uint16_t _count = children_count;
	pack(_count, ptr);

	local.PackToBin(ptr);
}

/************************************************************************/
/* struct PackAnim2::Skin                                               */
/************************************************************************/

PackAnim2::Skin::
Skin()
	: node(NULL)
	, type(0)
{
}

PackAnim2::Skin::
~Skin()
{
	if (node) {
		node->RemoveReference();
	}
}

void PackAnim2::Skin::
PackToLuaString(ebuilder::CodeGenerator& gen) const
{
	lua::connect(gen, 2,
		lua::assign("symbol", node->GetID()),
		lua::assign("type", type));
	local.PackToLuaString(gen);
}

int PackAnim2::Skin::
SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);		// node
	sz += sizeof(uint8_t);		// type
	sz += local.SizeOfPackToBin();
	return sz;
}

void PackAnim2::Skin::
PackToBin(uint8_t** ptr) const
{
	uint32_t id = node->GetID();
	pack(id, ptr);

	uint8_t t = type;
	pack(t, ptr);

	local.PackToBin(ptr);
}

/************************************************************************/
/* struct PackAnim2::Slot                                               */
/************************************************************************/

void PackAnim2::Slot::
PackToLuaString(ebuilder::CodeGenerator& gen) const
{
	lua::connect(gen, 2,
		lua::assign("joint", joint),
		lua::assign("skin", skin));
}

int PackAnim2::Slot::
SizeOfPackToBin() const
{
	return sizeof(uint16_t) * 2;
}

void PackAnim2::Slot::
PackToBin(uint8_t** ptr) const
{
	uint16_t val;
	val = joint;
	pack(val, ptr);
	val = skin;
	pack(val, ptr);
}

/************************************************************************/
/* struct PackAnim2::JointSample                                        */
/************************************************************************/

void PackAnim2::JointSample::
PackToLuaString(ebuilder::CodeGenerator& gen) const
{
	lua::connect(gen, 3,
		lua::assign("time", time),
		lua::assign("lerp", lerp),
		lua::assign("data", data));
}

int PackAnim2::JointSample::
SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint16_t);	// time
	sz += sizeof(uint8_t);	// lerp
	sz += sizeof(float);    // data
	return sz;
}

void PackAnim2::JointSample::
PackToBin(uint8_t** ptr) const
{
	uint16_t _time = time;
	pack(_time, ptr);

	uint8_t _padding = padding;
	pack(_padding, ptr);

	float _data = data;
	pack(_data, ptr);
}

/************************************************************************/
/* struct PackAnim2::TL_Joint                                           */
/************************************************************************/

void PackAnim2::TL_Joint::
PackToLuaString(ebuilder::CodeGenerator& gen) const
{
	lua::assign_with_end(gen, "type", type);
	
	lua::connect(gen, 7,
		lua::assign("trans_x",	dims_count[0]),
		lua::assign("trans_y",	dims_count[1]),
		lua::assign("rot",		dims_count[2]),
		lua::assign("scale_x",	dims_count[3]),
		lua::assign("scale_y",	dims_count[4]),
		lua::assign("shear_x",	dims_count[5]),
		lua::assign("shear_y",	dims_count[6]));

	{
		lua::TableAssign ta(gen, "samples", true);
		for (int i = 0, n = samples.size(); i < n; ++i) {
			samples[i].PackToLuaString(gen);
		}
	}
}

int PackAnim2::TL_Joint::
SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint8_t);				// type
	sz += sizeof(uint8_t) * DIM_COUNT;	// dims_count
	for (int i = 0, n = samples.size(); i < n; ++i) {
		sz += samples[i].SizeOfPackToBin();
	}
	return sz;
}

void PackAnim2::TL_Joint::
PackToBin(uint8_t** ptr) const
{
	uint8_t _type = type;
	pack(_type, ptr);

	for (int i = 0; i < DIM_COUNT; ++i) {
		uint8_t count = dims_count[i];
		pack(count, ptr);
	}

	for (int i = 0, n = samples.size(); i < n; ++i) {
		samples[i].PackToBin(ptr);
	}
}

/************************************************************************/
/* struct PackAnim2::SkinSample                                         */
/************************************************************************/

void PackAnim2::SkinSample::
PackToLuaString(ebuilder::CodeGenerator& gen) const
{
	lua::connect(gen, 2,
		lua::assign("time", time),
		lua::assign("skin", skin));	
}

int PackAnim2::SkinSample::
SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint16_t); // time
	sz += sizeof(uint16_t); // skin
	return sz;
}

void PackAnim2::SkinSample::
PackToBin(uint8_t** ptr) const
{
	uint16_t _time = time;
	pack(_time, ptr);
	uint16_t _skin = skin;
	pack(_skin, ptr);
}

/************************************************************************/
/* struct PackAnim2::TL_Skin                                            */
/************************************************************************/

void PackAnim2::TL_Skin::
PackToLuaString(ebuilder::CodeGenerator& gen) const
{
	lua::TableAssign ta(gen, "samples", true);
	for (int i = 0, n = samples.size(); i < n; ++i) {
		samples[i].PackToLuaString(gen);
	}	
}

int PackAnim2::TL_Skin::
SizeOfPackToBin() const
{
	int sz = 0;
	for (int i = 0, n = samples.size(); i < n; ++i) {
		sz += samples[i].SizeOfPackToBin();
	}
	return sz;
}

void PackAnim2::TL_Skin::
PackToBin(uint8_t** ptr) const
{
	for (int i = 0, n = samples.size(); i < n; ++i) {
		samples[i].PackToBin(ptr);
	}
}

/************************************************************************/
/* struct PackAnim2::DeformSample                                       */
/************************************************************************/

void PackAnim2::DeformSample::
PackToLuaString(ebuilder::CodeGenerator& gen) const
{
	lua::connect(gen, 2,
		lua::assign("time", time),
		lua::assign("offset", offset));	

	PackCoords::PackToLua(gen, data, "data");
}

int PackAnim2::DeformSample::
SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint16_t);		// time
	sz += sizeof(uint16_t);		// offset
	// data
	sz += sizeof(uint16_t);
	sz += sizeof(float) * 2 * data.size();
	return sz;
}

void PackAnim2::DeformSample::
PackToBin(uint8_t** ptr) const
{
	uint16_t _time = time;
	pack(_time, ptr);

	uint16_t _offset = offset;
	pack(_offset, ptr);

	uint16_t count = data.size();
	pack(count, ptr);
	float f;
	for (int i = 0; i < count; ++i) {
		f = data[i].x;
		pack(f, ptr);
		f = data[i].y;
		pack(f, ptr);
	}
}

/************************************************************************/
/* struct PackAnim2::TL_Deform                                          */
/************************************************************************/

void PackAnim2::TL_Deform::
PackToLuaString(ebuilder::CodeGenerator& gen) const
{
	lua::TableAssign ta(gen, "deforms", true);
	for (int i = 0, n = deforms.size(); i < n; ++i) {
		deforms[i].PackToLuaString(gen);
	}	
}

int PackAnim2::TL_Deform::
SizeOfPackToBin() const
{
	int sz = 0;
	for (int i = 0, n = deforms.size(); i < n; ++i) {
		sz += deforms[i].SizeOfPackToBin();
	}
	return sz;
}

void PackAnim2::TL_Deform::
PackToBin(uint8_t** ptr) const
{
	for (int i = 0, n = deforms.size(); i < n; ++i) {
		deforms[i].PackToBin(ptr);
	}	
}

}