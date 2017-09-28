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
#include <sprite2/LerpCircle.h>
#include <sprite2/LerpSpiral.h>
#include <sprite2/LerpWiggle.h>
#include <sprite2/LerpEase.h>

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

void PackAnimation::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp) const
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

void PackAnimation::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp) const
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
	const auto& layers = sym->GetLayers();
	m_layers.reserve(layers.size());
	for (int i = 0, n = layers.size(); i < n; ++i) {
		m_layers.push_back(new Layer(*layers[i]));
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
/* class PackAnimation::Lerp                                            */
/************************************************************************/

PackAnimation::Lerp::
Lerp(s2::AnimLerp::SprData type, const s2::ILerp& data)
	: m_spr_data(type)
	, m_lerp(data.Clone())
{
	int t = m_lerp->Type();
}

void PackAnimation::Lerp::
PackToLuaString(ebuilder::CodeGenerator& gen) const
{
	lua::TableAssign assign(gen, "", true);

	int t = m_lerp->Type();

	lua::connect(gen, 2, 
		lua::assign("spr_data", m_spr_data), 
		lua::assign("lerp", m_lerp->Type()));
	switch (m_lerp->Type())
	{
	case s2::LERP_CIRCLE:
		{
			const s2::LerpCircle& lerp = static_cast<const s2::LerpCircle&>(*m_lerp);
			lua::connect(gen, 1, 
				lua::assign("scale", lerp.GetScale()));			
		}
		break;
	case s2::LERP_SPIRAL:
		{
			const s2::LerpSpiral& lerp = static_cast<const s2::LerpSpiral&>(*m_lerp);
			float begin, end;
			lerp.GetAngle(begin, end);
			lua::connect(gen, 3, 
				lua::assign("angle begin", begin),
				lua::assign("angle end", end),
				lua::assign("scale", lerp.GetScale()));		
		}
		break;
	case s2::LERP_WIGGLE:
		{
			const s2::LerpWiggle& lerp = static_cast<const s2::LerpWiggle&>(*m_lerp);
			lua::connect(gen, 2, 
				lua::assign("freq", lerp.GetFreq()),
				lua::assign("amp", lerp.GetAmp()));
		}
		break;
	case s2::LERP_EASE:
		{
			const s2::LerpEase& lerp = static_cast<const s2::LerpEase&>(*m_lerp);
			lua::connect(gen, 1, 
				lua::assign("ease", lerp.GetEaseType()));
		}
		break;
	}
}

int PackAnimation::Lerp::
SizeOfUnpackFromBin() const
{
	int sz = simp::NodeAnimation::LerpSize();
	sz += sizeof(uint32_t) * GetLerpDataSize(m_lerp->Type());
	return sz;
}

int PackAnimation::Lerp::
SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint16_t);	// spr data
	sz += sizeof(uint16_t);	// lerp type
	sz += sizeof(uint16_t);	// data n
	sz += sizeof(uint32_t) * GetLerpDataSize(m_lerp->Type()); // data
	return sz;
}

void PackAnimation::Lerp::
PackToBin(uint8_t** ptr) const
{
	uint16_t spr_data = m_spr_data;
	pack(spr_data, ptr);
	uint16_t lerp_type = m_lerp->Type();
	pack(lerp_type, ptr);
	uint16_t data_n = GetLerpDataSize(lerp_type);
	pack(data_n, ptr);
	switch (m_lerp->Type())
	{
	case s2::LERP_CIRCLE:
		{
			const s2::LerpCircle& lerp = static_cast<const s2::LerpCircle&>(*m_lerp);
			float scale = lerp.GetScale();
			pack(scale, ptr);
		}
		break;
	case s2::LERP_SPIRAL:
		{
			const s2::LerpSpiral& lerp = static_cast<const s2::LerpSpiral&>(*m_lerp);
			float begin, end;
			lerp.GetAngle(begin, end);
			pack(begin, ptr);
			pack(end, ptr);
			float scale = lerp.GetScale();
			pack(scale, ptr);
		}
		break;
	case s2::LERP_WIGGLE:
		{
			const s2::LerpWiggle& lerp = static_cast<const s2::LerpWiggle&>(*m_lerp);
			float freq = lerp.GetFreq();
			float amp = lerp.GetAmp();
			pack(freq, ptr);
			pack(amp, ptr);
		}
		break;
	case s2::LERP_EASE:
		{
			const s2::LerpEase& lerp = static_cast<const s2::LerpEase&>(*m_lerp);
			uint32_t ease_type = lerp.GetEaseType();
			pack(ease_type, ptr);
		}
		break;
	}
}

int PackAnimation::Lerp::
GetLerpDataSize(int lerp_type)
{
	int size = 0;
	switch (lerp_type)
	{
	case s2::LERP_CIRCLE:
		size = 1;
		break;
	case s2::LERP_SPIRAL:
		size = 3;
		break;
	case s2::LERP_WIGGLE:
		size = 2;
		break;
	case s2::LERP_EASE:
		size = 1;
		break;
	}
	return size;
}

/************************************************************************/
/* class PackAnimation::Frame                                           */
/************************************************************************/

PackAnimation::Frame::
Frame(const s2::AnimSymbol::Frame& frame)
{
	m_index = frame.index;
	m_tween = frame.tween;

	m_actors.reserve(frame.sprs.size());
	for (int spr = 0, spr_n = frame.sprs.size(); spr < spr_n; ++spr) {
		m_actors.push_back(new Actor(frame.sprs[spr]));
	}

	m_lerps.reserve(frame.lerps.size());
	for (int i = 0, n = frame.lerps.size(); i < n; ++i) {
		const auto& src = frame.lerps[i];
		Lerp* dst = new Lerp(src.first, *src.second);
		m_lerps.push_back(dst);
	}
}

PackAnimation::Frame::
~Frame()
{
	for_each(m_actors.begin(), m_actors.end(), cu::RemoveRefFunctor<Actor>());
	for_each(m_lerps.begin(), m_lerps.end(), cu::RemoveRefFunctor<Lerp>());
}

void PackAnimation::Frame::
PackToLuaString(ebuilder::CodeGenerator& gen) const
{
	lua::TableAssign assign(gen, "", true);
	lua::connect(gen, 2, 
		lua::assign("index", m_index), 
		lua::assign("tween", m_tween));	
	{
		lua::TableAssign ta(gen, "actors", true);
		for (int i = 0, n = m_actors.size(); i < n; ++i) {
			m_actors[i]->PackToLuaString(gen);
		}
	}
	{
		lua::TableAssign ta(gen, "lerps", true);
		for (int i = 0, n = m_lerps.size(); i < n; ++i) {
			m_lerps[i]->PackToLuaString(gen);
		}
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
	sz += m_lerps.size() * simp::SIZEOF_POINTER;
	for (int i = 0, n = m_lerps.size(); i < n; ++i) {
		sz += m_lerps[i]->SizeOfUnpackFromBin();
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
	// lerps
	sz += sizeof(uint16_t);
	for (int i = 0, n = m_lerps.size(); i < n; ++i) {
		sz += m_lerps[i]->SizeOfPackToBin();
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

	uint16_t actor_n = m_actors.size();
	pack(actor_n, ptr);
	for (int i = 0; i < actor_n; ++i) {
		m_actors[i]->PackToBin(ptr);
	}

	uint16_t lerp_n = m_lerps.size();
	pack(lerp_n, ptr);
	for (int i = 0; i < lerp_n; ++i) {
		m_lerps[i]->PackToBin(ptr);
	}
}

/************************************************************************/
/* class PackAnimation::Layer                                           */
/************************************************************************/

PackAnimation::Layer::
Layer(const s2::AnimSymbol::Layer& layer)
{
	m_frames.reserve(layer.frames.size());
	for (int frame = 0, frame_n = layer.frames.size(); frame < frame_n; ++frame)
	{
		const auto& src_frame = layer.frames[frame];
		Frame* dst_frame = new Frame(*src_frame);
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