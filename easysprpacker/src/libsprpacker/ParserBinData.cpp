#include "ParserBinData.h"

#include <cu/cu_stl.h>
#include <simp/Package.h>
#include <simp/PkgIdxLoader.h>
#include <simp/Page.h>
#include <simp/PageAlloc.h>
#include <simp/simp_define.h>
#include <simp/simp_types.h>
#include <simp/NodeID.h>

#include <simp/NodeScale9Spr.h>
#include <simp/NodeIconSpr.h>
#include <simp/NodeTextureSpr.h>
#include <simp/NodeComplexSpr.h>
#include <simp/NodeAnimationSpr.h>
#include <simp/NodeParticle3dSpr.h>
#include <simp/NodeParticle2dSpr.h>
#include <simp/NodeShapeSpr.h>
#include <simp/NodeMeshSpr.h>
#include <simp/NodeMaskSpr.h>
#include <simp/NodeTrailSpr.h>
#include <simp/NodeAnim2Spr.h>
#include <simp/NodeAudioSpr.h>

namespace esprpacker
{

void ParserBinData::LoadFromFile(const std::string& filepath)
{
	bimp::Allocator* alloc = simp::PageAlloc::Instance()->Create(simp::SIMP_PAGE_SIZE_MAX);
	if (!alloc) {
		return;
	}

	simp::Package* pkg = new simp::Package(filepath.c_str(), -1);
	int version = pkg->GetVersion();
	auto& pages = pkg->GetPages();
	int idx = 1;
	for (auto& page : pages)
	{
		auto page_data = std::make_unique<simp::Page>(-1, version, alloc, page.MinID(), page.MaxID());
		std::string page_path = filepath;
		page_path.insert(page_path.rfind(".epe"), "." + std::to_string(idx));
		page_data->Load(bimp::FilePath(page_path.c_str()));
		NodeVisitor visitor(m_spr_nodes);
		page_data->Traverse(visitor);

		++idx;
	}
}

void ParserBinData::InitSprIDSet(int pkg_id)
{
	for (auto& spr_array : m_spr_nodes) {
		for (auto& spr : spr_array.second) {
			m_spr_id_set.insert(simp::NodeID::ComposeID(pkg_id, spr.id));
		}
	}
}

bool ParserBinData::IsIdUsed(uint32_t id) const
{
	return m_spr_id_set.find(id) != m_spr_id_set.end();
}

//////////////////////////////////////////////////////////////////////////
// class ParserBinData::NodeVisitor
//////////////////////////////////////////////////////////////////////////

void ParserBinData::NodeVisitor::Visit(int id, int type, const void* node)
{
	switch (type)
	{
	case simp::TYPE_SCALE9_SPR:
	{
		auto spr = static_cast<const simp::NodeScale9Spr*>(node);
		InsertSpr(spr->sym, id , node);
		break;
	}
	case simp::TYPE_ICON_SPR:
	{
		auto spr = static_cast<const simp::NodeIconSpr*>(node);
		InsertSpr(spr->sym, id , node);
		break;
	}
	case simp::TYPE_TEXTURE_SPR:
	{
		auto spr = static_cast<const simp::NodeTextureSpr*>(node);
		InsertSpr(spr->sym, id , node);
		break;
	}
	case simp::TYPE_COMPLEX_SPR:
	{
		auto spr = static_cast<const simp::NodeComplexSpr*>(node);
		InsertSpr(spr->sym, id , node);
		break;
	}
	case simp::TYPE_ANIM_SPR:
	{
		auto spr = static_cast<const simp::NodeAnimationSpr*>(node);
		InsertSpr(spr->sym, id , node);
		break;
	}
	case simp::TYPE_PARTICLE3D:
	{
		auto spr = static_cast<const simp::NodeParticle3dSpr*>(node);
		InsertSpr(spr->sym, id , node);
		break;
	}
	case simp::TYPE_PARTICLE2D:
	{
		auto spr = static_cast<const simp::NodeParticle2dSpr*>(node);
		InsertSpr(spr->sym, id , node);
		break;
	}
	case simp::TYPE_SHAPE_SPR:
	{
		auto spr = static_cast<const simp::NodeShapeSpr*>(node);
		InsertSpr(spr->sym, id , node);
		break;
	}
	case simp::TYPE_MESH_SPR:
	{
		auto spr = static_cast<const simp::NodeMeshSpr*>(node);
		InsertSpr(spr->mesh_id, id , node);
		break;
	}
	case simp::TYPE_MASK_SPR:
	{
		auto spr = static_cast<const simp::NodeMaskSpr*>(node);
		InsertSpr(spr->sym, id , node);
		break;
	}
	case simp::TYPE_TRAIL_SPR:
	{
		auto spr = static_cast<const simp::NodeTrailSpr*>(node);
		InsertSpr(spr->sym, id , node);
		break;
	}
	case simp::TYPE_ANIM2_SPR:
	{
		auto spr = static_cast<const simp::NodeAnim2Spr*>(node);
		InsertSpr(spr->sym, id , node);
		break;
	}
	case simp::TYPE_AUDIO_SPR:
	{
		auto spr = static_cast<const simp::NodeAudioSpr*>(node);
		InsertSpr(spr->sym, id , node);
		break;
	}
	}
}

void ParserBinData::NodeVisitor::InsertSpr(uint32_t sym_id, int spr_id, const void* spr_node)
{
	auto itr = m_spr_nodes.find(sym_id);
	if (itr != m_spr_nodes.end()) {
		itr->second.emplace_back(spr_id, spr_node);
	} else {
		std::vector<ParserBinData::NodeSpr> nodes;
		nodes.emplace_back(spr_id, spr_node);
		m_spr_nodes.insert(std::make_pair(sym_id, nodes));
	}
}

}