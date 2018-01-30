#include "s2loader/Anim2SymLoader.h"
#include "s2loader/EasyAnim2Loader.h"
#include "s2loader/SpineAnim2Loader.h"
#include "s2loader/ExtendSymFile.h"

#include <sprite2/Anim2Symbol.h>
#include <simp/NodeAnim2.h>
#include <bimp/typedef.h>
#include <gum/FilepathHelper.h>
#include <gum/SymbolPool.h>

#include <fstream>

#include <string.h>
#include <assert.h>

namespace s2loader
{

Anim2SymLoader::Anim2SymLoader(s2::Anim2Symbol& sym,
							   const std::shared_ptr<const SymbolLoader>& sym_loader)
	: m_sym(sym)
	, m_sym_loader(sym_loader)
{
}

void Anim2SymLoader::LoadJson(const CU_STR& filepath)
{
	CU_STR dir = gum::FilepathHelper::Dir(filepath);

	Json::Value val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	int type = ExtendSymFile::GetType(val);
	switch (type)
	{
	case SYM_SPINE:
		{
			SpineAnim2Loader loader(m_sym, m_sym_loader);
			loader.LoadJson(val, dir);
		}
		break;
	case SYM_UNKNOWN:
		{
			EasyAnim2Loader loader(m_sym, m_sym_loader);
			loader.LoadJson(val, dir);
		}
		break;
	}
}

static void LoadBinSRT(rg_pose_srt& dst, const simp::NodeAnim2::Srt& src)
{
	dst.trans[0] = src.trans.x;
	dst.trans[1] = src.trans.y;
	dst.rot      = src.rot;
	dst.scale[0] = src.scale.x;
	dst.scale[1] = src.scale.y;
}

void Anim2SymLoader::LoadBin(const simp::NodeAnim2* node)
{
	int sz = CalcNodeSize(node);
	void* buf = malloc(sz);
	uint8_t* ptr = static_cast<uint8_t*>(buf);

	rg_animation* anim = (rg_animation*)ptr;
	anim->max_frame = 0;
	ptr += SIZEOF_RG_ANIM;
	// skeleton
	rg_skeleton* sk = (rg_skeleton*)ptr;	
	anim->sk = sk;
	ptr += SIZEOF_RG_SKELETON;
	sk->joint_count = node->joint_count;
	sk->ik_count = node->ik_count;
	sk->root = node->root;
	sk->slot_count = node->slot_count;
	sk->skin_count = node->skin_count;
	//// skins
	m_sym.ClearCachedSym();
	for (int i = 0; i < sk->skin_count; ++i) 
	{
		const simp::NodeAnim2::Skin& src = node->skins[i];
		rg_skin* dst = &sk->skins[i];
		dst->type = src.type;
		LoadBinSRT(dst->local, src.local);
		auto sym = gum::SymbolPool::Instance()->Fetch(src.node);
		m_sym.AddCachedSym(sym);
		dst->ud = static_cast<void*>(sym.get());
		ptr += SIZEOF_RG_SKIN;
	}
	//// slots
	sk->slots = (rg_slot*)ptr;
	for (int i = 0; i < sk->slot_count; ++i)
	{
		sk->slots[i].joint = node->slots[i].joint;
		sk->slots[i].skin  = node->slots[i].skin;
	}
	ptr += SIZEOF_RG_SLOT * sk->slot_count;
	//// iks
	sk->iks = (rg_ik*)ptr;
	for (int i = 0; i < sk->ik_count; ++i)
	{
		const simp::NodeAnim2::IK& src = node->iks[i];
		rg_ik* dst = &sk->iks[i];
		dst->joints[0] = src.joints[0];
		dst->joints[1] = src.joints[1];
		dst->target = src.target;
		dst->bend_positive = src.bend_positive;
		dst->length[0] = src.length[0];
		dst->length[1] = src.length[1];
	}
	ptr += SIZEOF_RG_IK * sk->ik_count;
	//// joints
	sk->joints = (rg_joint**)ptr;
	ptr += sizeof(rg_joint*) * sk->joint_count;
	for (int i = 0; i < sk->joint_count; ++i) 
	{
		sk->joints[i] = (rg_joint*)ptr;
		ptr += SIZEOF_RG_JOINT;

		const simp::NodeAnim2::Joint& src = node->joints[i];
		rg_joint* dst = sk->joints[i];
		rg_pose_mat_identity(&dst->world_pose);
		LoadBinSRT(dst->local_pose, src.local);
		dst->parent = src.parent;
		dst->children_count = src.chlidren_count;
		
		ptr += ALIGN_4BYTE(sizeof(uint16_t) * dst->children_count);
	}
	InitJointChildren(sk);
	// timeline
	//// tl joints
	anim->timeline.joints = (rg_tl_joint**)ptr;
	ptr += sizeof(rg_tl_joint*) * sk->joint_count;
	for (int i = 0; i < sk->joint_count; ++i)
	{
		anim->timeline.joints[i] = (rg_tl_joint*)ptr;
		ptr += SIZEOF_RG_TIMELINE_JOINT;

		const simp::NodeAnim2::TL_Joint& src = *node->tl_joints[i];
		rg_tl_joint* dst = anim->timeline.joints[i];
		dst->type = src.type;
		int count = 0;
		for (int j = 0; j < simp::NodeAnim2::DIM_COUNT; ++j) {
			dst->dims_count[j] = src.dims_count[j];
			count += dst->dims_count[j];
		}
		for (int j = 0; j < count; ++j) 
		{
			dst->samples[j].time  = src.samples[j].time;
			dst->samples[j].lerp  = src.samples[j].lerp;
			dst->samples[j].curve = src.samples[j].curve;
			dst->samples[j].data  = src.samples[j].data;
			if (dst->samples[j].time > anim->max_frame) {
				anim->max_frame = dst->samples[j].time;
			}
		}
		ptr += SIZEOF_RG_JOINT_SAMPLE * count;
	}
	//// tl skins
	anim->timeline.skins = (rg_tl_skin**)ptr;
	ptr += sizeof(rg_tl_skin*) * sk->slot_count;
	for (int i = 0; i < sk->slot_count; ++i)
	{
		anim->timeline.skins[i] = (rg_tl_skin*)ptr;
		ptr += SIZEOF_RG_TIMELINE_SKIN;

		const simp::NodeAnim2::TL_Skin& src = *node->tl_skins[i];
		rg_tl_skin* dst = anim->timeline.skins[i];
		dst->skin_count = src.count;
		for (int j = 0; j < dst->skin_count; ++j) {
			dst->skins[j].time = src.samples[j].time;
			dst->skins[j].skin = src.samples[j].skin;
		}

		ptr += SIZEOF_RG_SKIN_SAMPLE * dst->skin_count;
	}
	//// tl deforms
	anim->timeline.deforms = (rg_tl_deform**)ptr;
	ptr += sizeof(rg_tl_deform*) * sk->skin_count;
	for (int i = 0; i < sk->skin_count; ++i)
	{
		anim->timeline.deforms[i] = (rg_tl_deform*)ptr;
		ptr += SIZEOF_RG_TIMELINE_DEFORM;

		const simp::NodeAnim2::TL_Deform& src = *node->tl_deforms[i];
		rg_tl_deform* dst = anim->timeline.deforms[i];
		dst->count = src.count;
		for (int j = 0; j < dst->count; ++j) 
		{
			dst->samples[j].time   = src.samples[j]->time;
			dst->samples[j].offset = src.samples[j]->offset;
			dst->samples[j].curve  = src.samples[j]->curve;
			dst->samples[j].count  = src.samples[j]->count;
			dst->samples[j].data   = nullptr;
		}
		ptr += SIZEOF_RG_DEFORM_SAMPLE * dst->count;

		for (int j = 0; j < dst->count; ++j) 
		{
			if (dst->samples[j].count == 0) {
				dst->samples[j].data = nullptr;
			} else {
				dst->samples[j].data = (float*)ptr;
				int sz = sizeof(float) * 2 * dst->samples[j].count;
				memcpy(dst->samples[j].data, src.samples[j]->vertices, sz);
				ptr += sz;
			}
		}
	}
	//// curves
	anim->curve_count = node->curve_count;
	anim->curves = (rg_curve**)ptr;
	ptr += sizeof(rg_curve*) * anim->curve_count;
	for (int i = 0; i < anim->curve_count; ++i)
	{
		anim->curves[i] = (rg_curve*)ptr;
		ptr += SIZEOF_RG_CURVE;
		
		auto& src = node->curves[i];
		auto& dst = anim->curves[i];
		dst->x0 = src.x0;
		dst->y0 = src.y0;
		dst->x1 = src.x1;
		dst->y1 = src.y1;
	}
	assert(ptr - static_cast<uint8_t*>(buf) == sz);
	m_sym.SetAnim(anim);
}

int Anim2SymLoader::CalcNodeSize(const simp::NodeAnim2* node)
{
	int sz = 0;
	// rg_animation
	sz += SIZEOF_RG_ANIM;
	// rg_skeleton
	sz += SIZEOF_RG_SKELETON;
	// joints
	sz += sizeof(rg_joint*) * node->joint_count;
	for (int i = 0; i < node->joint_count; ++i) {
		sz += SIZEOF_RG_JOINT;
		sz += ALIGN_4BYTE(sizeof(uint16_t) * node->joints[i].chlidren_count);
	}
	// iks
	sz += SIZEOF_RG_IK * node->ik_count;
	// slots
	sz += SIZEOF_RG_SLOT * node->slot_count;
	// skins
	sz += SIZEOF_RG_SKIN * node->skin_count;
	// tl joints
	sz += sizeof(rg_tl_joint*) * node->joint_count;
	for (int i = 0; i < node->joint_count; ++i) 
	{
		sz += SIZEOF_RG_TIMELINE_JOINT;
		simp::NodeAnim2::TL_Joint* src = node->tl_joints[i];
		int count = 0;
		for (int j = 0; j < simp::NodeAnim2::DIM_COUNT; ++j) {
			count += src->dims_count[j];
		}
		sz += SIZEOF_RG_JOINT_SAMPLE * count;
	}
	// tl skins
	sz += sizeof(rg_tl_skin*) * node->slot_count;
	for (int i = 0; i < node->slot_count; ++i)
	{
		sz += SIZEOF_RG_TIMELINE_SKIN;
		sz += SIZEOF_RG_SKIN_SAMPLE * node->tl_skins[i]->count;
	}
	// tl deforms
	sz += sizeof(rg_tl_deform*) * node->skin_count;
	for (int i = 0; i < node->skin_count; ++i)
	{
		sz += SIZEOF_RG_TIMELINE_DEFORM;
		for (int j = 0; j < node->tl_deforms[i]->count; ++j) {
			sz += SIZEOF_RG_DEFORM_SAMPLE;
 			sz += sizeof(float) * 2 * node->tl_deforms[i]->samples[j]->count;
		}
	}
	// curves
	sz += sizeof(rg_curve*) * node->curve_count;
	sz += SIZEOF_RG_CURVE * node->curve_count;
	return sz;
}

void Anim2SymLoader::InitJointChildren(rg_skeleton* sk)
{
	CU_VEC<int> node_tag;
	node_tag.resize(sk->joint_count, 0);
	for (int i = 0; i < sk->joint_count; ++i) {
		int parent = sk->joints[i]->parent;
		if (parent != RG_JOINT_UNKNOWN) {
			sk->joints[parent]->children[node_tag[parent]++] = i;
		}
	}
}

}