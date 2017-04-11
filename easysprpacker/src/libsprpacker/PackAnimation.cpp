#include "PackAnimation.h"
#include "PackNodeFactory.h"
#include "binary_io.h"
#include "to_int.h"

#include <easyanim.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp/NodeAnimation.h>
#include <simp/simp_define.h>
#include <simp/simp_types.h>

#include <algorithm>

namespace esprpacker
{

PackAnimation::PackAnimation(const libanim::Symbol* sym)
{
	Init(sym);
}

PackAnimation::~PackAnimation()
{
	for_each(m_layers.begin(), m_layers.end(), cu::RemoveRefFunctor<Layer>());
}

void PackAnimation::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"animation\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(GetID()));
	if (!GetName().empty()) {
		lua::assign_with_end(gen, "export", "\"" + GetName() + "\"");
	}

	lua::TableAssign ta(gen, "layers", true);
	for (int i = 0, n = m_layers.size(); i < n; ++i) {
		m_layers[i]->PackToLuaString(gen);
	}

	gen.detab();
	gen.line("},");
}

int PackAnimation::SizeOfUnpackFromBin() const
{
	int sz = simp::NodeAnimation::Size();
	for (int i = 0, n = m_layers.size(); i < n; ++i) {
		sz += m_layers[i]->SizeOfUnpackFromBin();
	}
	return sz;
}

int PackAnimation::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);			// id
	sz += sizeof(uint8_t);			// type
	// layers
	sz += sizeof(uint16_t);
	for (int i = 0, n = m_layers.size(); i < n; ++i) {
		sz += m_layers[i]->SizeOfPackToBin();
	}
	return sz;
}

void PackAnimation::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = GetID();
	pack(id, ptr);

	uint8_t type = simp::TYPE_ANIMATION;
	pack(type, ptr);

	// layers
	uint16_t n = m_layers.size();
	pack(n, ptr);
	for (int i = 0; i < n; ++i) {
		m_layers[i]->PackToBin(ptr);
	}
}

void PackAnimation::Init(const libanim::Symbol* sym)
{
	const std::vector<s2::AnimSymbol::Layer*>& layers = sym->GetLayers();
	m_layers.reserve(layers.size());
	for (int i = 0, n = layers.size(); i < n; ++i) {
		m_layers.push_back(new Layer(layers[i]));
	}

	CheckLerp(sym->GetFilepath());
}

void PackAnimation::CheckLerp(const std::string& filepath)
{
	for (int i = 0, n = m_layers.size(); i < n; ++i) 
	{
		Layer* layer = m_layers[i];
		for (int j = 0, m = layer->m_frames.size(); j < m - 1; ++j) 
		{
			Frame *curr = layer->m_frames[j],
				  *next = layer->m_frames[j + 1];
			if (!curr->m_tween) {
				continue;
			}

			for (int curr_idx = 0; curr_idx < curr->m_actors.size(); ++curr_idx) {
				int count = 0;
				std::string name;
				for (int next_idx = 0; next_idx < next->m_actors.size(); ++next_idx) {
					const std::string& curr_name = curr->m_actors[curr_idx]->m_trans.GetName();
					const std::string& next_name = next->m_actors[next_idx]->m_trans.GetName();
					if (curr_name == next_name) {
						name = curr_name;
						++count;
					}
				}
				if (count > 1) {
					throw ee::Exception("anim lerp error! filepath %s, layer %d, frame %d, name %s\n", 
						filepath.c_str(), i, j, name.c_str());
				}
			}
		}
	}
}

/************************************************************************/
/* class PackAnimation::Actor                                           */
/************************************************************************/

PackAnimation::Actor::
Actor(const s2::Sprite* spr)
	: m_trans(*spr, true)
{
	m_node = PackNodeFactory::Instance()->Create(dynamic_cast<const ee::Sprite*>(spr));
}

PackAnimation::Actor::
~Actor()
{
	m_node->RemoveReference();
}

void PackAnimation::Actor::
PackToLuaString(ebuilder::CodeGenerator& gen) const
{
	lua::TableAssign assign(gen, "", true);

	std::string sid = lua::assign("id", ee::StringHelper::ToString(m_node->GetID()));
	lua::connect(gen, 1, sid);
	m_trans.PackToLua(gen);
}

int PackAnimation::Actor::
SizeOfUnpackFromBin() const
{
	int sz = simp::NodeAnimation::ActorSize();
	sz += m_trans.SizeOfUnpackFromBin();
	return sz;
}

int PackAnimation::Actor::
SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);				// node id
	sz += m_trans.SizeOfPackToBin();	// trans
	return sz;
}

void PackAnimation::Actor::
PackToBin(uint8_t** ptr) const
{
	uint32_t id = m_node->GetID();
	pack(id, ptr);

	m_trans.PackToBin(ptr);
}

/************************************************************************/
/* class PackAnimation::Frame                                           */
/************************************************************************/

PackAnimation::Frame::
Frame(const s2::AnimSymbol::Frame* frame)
{
	m_index = frame->index;
	m_tween = frame->tween;
	m_actors.reserve(frame->sprs.size());
	for (int spr = 0, spr_n = frame->sprs.size(); spr < spr_n; ++spr) {
		m_actors.push_back(new Actor(frame->sprs[spr]));
	}
}

PackAnimation::Frame::
~Frame()
{
	for_each(m_actors.begin(), m_actors.end(), cu::RemoveRefFunctor<Actor>());
}

void PackAnimation::Frame::
PackToLuaString(ebuilder::CodeGenerator& gen) const
{
	lua::TableAssign assign(gen, "", true);

	lua::connect(gen, 2, 
		lua::assign("index", m_index), 
		lua::assign("tween", m_tween));
	
	lua::TableAssign ta(gen, "actors", true);
	for (int i = 0, n = m_actors.size(); i < n; ++i) {
		m_actors[i]->PackToLuaString(gen);
	}
}

int PackAnimation::Frame::
SizeOfUnpackFromBin() const
{
	int sz = simp::NodeAnimation::FrameSize();
	sz += m_actors.size() * simp::SIZEOF_POINTER;
	for (int i = 0, n = m_actors.size(); i < n; ++i) {
		sz += m_actors[i]->SizeOfUnpackFromBin();
	}
	return sz;
}

int PackAnimation::Frame::
SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint16_t);		// index
	sz += sizeof(uint8_t);		// tween
	// actors
	sz += sizeof(uint16_t);
	for (int i = 0, n = m_actors.size(); i < n; ++i) {
		sz += m_actors[i]->SizeOfPackToBin();
	}
	return sz;
}

void PackAnimation::Frame::
PackToBin(uint8_t** ptr) const
{
	uint16_t idx = m_index;
	pack(idx, ptr);

	uint8_t tween = bool2int(m_tween);
	pack(tween, ptr);

	uint16_t n = m_actors.size();
	pack(n, ptr);
	for (int i = 0; i < n; ++i) {
		m_actors[i]->PackToBin(ptr);
	}
}

/************************************************************************/
/* class PackAnimation::Layer                                           */
/************************************************************************/

PackAnimation::Layer::
Layer(const s2::AnimSymbol::Layer* layer)
{
	m_frames.reserve(layer->frames.size());
	for (int frame = 0, frame_n = layer->frames.size(); frame < frame_n; ++frame)
	{
		s2::AnimSymbol::Frame* src_frame = layer->frames[frame];
		Frame* dst_frame = new Frame(src_frame);
		m_frames.push_back(dst_frame);
	}
}

PackAnimation::Layer::
~Layer()
{
	for_each(m_frames.begin(), m_frames.end(), cu::RemoveRefFunctor<Frame>());
}

void PackAnimation::Layer::
PackToLuaString(ebuilder::CodeGenerator& gen) const
{
	lua::TableAssign ta(gen, "frames", true);
	for (int i = 0, n = m_frames.size(); i < n; ++i) {
		m_frames[i]->PackToLuaString(gen);
	}
}

int PackAnimation::Layer::
SizeOfUnpackFromBin() const
{
	int sz = simp::NodeAnimation::LayerSize();
	sz += m_frames.size() * simp::SIZEOF_POINTER;
	for (int i = 0, n = m_frames.size(); i < n; ++i) {
		sz += m_frames[i]->SizeOfUnpackFromBin();
	}
	return sz;
}

int PackAnimation::Layer::
SizeOfPackToBin() const
{
	int sz = 0;
	// frames
	sz += sizeof(uint16_t);
	for (int i = 0, n = m_frames.size(); i < n; ++i) {
		sz += m_frames[i]->SizeOfPackToBin();
	}
	return sz;
}

void PackAnimation::Layer::
PackToBin(uint8_t** ptr) const
{
	uint16_t n = m_frames.size();
	pack(n, ptr);
	for (int i = 0; i < n; ++i) {
		m_frames[i]->PackToBin(ptr);
	}
}

}