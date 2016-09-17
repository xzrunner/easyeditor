#include "PackAnimation.h"
#include "PackNodeFactory.h"

#include <easyanim.h>

#include <algorithm>

namespace esprpacker
{

PackAnimation::PackAnimation(const eanim::Symbol* sym)
{
	Init(sym);
}

PackAnimation::~PackAnimation()
{
	for_each(m_layers.begin(), m_layers.end(), cu::RemoveRefFonctor<Layer>());
}

void PackAnimation::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	
}

int PackAnimation::SizeOfUnpackFromBin() const
{
	
}

int PackAnimation::SizeOfPackToBin() const
{

}

void PackAnimation::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{

}

void PackAnimation::Init(const eanim::Symbol* sym)
{
	const std::vector<s2::AnimSymbol::Layer*>& layers = sym->GetLayers();
	m_layers.reserve(layers.size());
	for (int i = 0, n = layers.size(); i < n; ++i) {
		m_layers.push_back(new Layer(layers[i]));
	}
}

/************************************************************************/
/* class PackAnimation::Frame                                           */
/************************************************************************/

PackAnimation::Frame::
Frame(const s2::AnimSymbol::Frame* frame)
{
	m_index = frame->index;
	m_tween = frame->tween;
	m_sprs.reserve(frame->sprs.size());
	for (int spr = 0, spr_n = frame->sprs.size(); spr < spr_n; ++spr)
	{
		ee::Sprite* src_spr = dynamic_cast<ee::Sprite*>(frame->sprs[spr]);
		const PackNode* dst_spr = PackNodeFactory::Instance()->Create(src_spr);
		m_sprs.push_back(dst_spr);
	}
}

PackAnimation::Frame::
~Frame()
{
	for_each(m_sprs.begin(), m_sprs.end(), cu::RemoveRefFonctor<PackNode>());
}

int PackAnimation::Frame::
SizeOfUnpackFromBin() const
{
	int sz = 
}

int PackAnimation::Frame::
SizeOfPackToBin() const
{

}

void PackAnimation::Frame::
PackToBin(uint8_t** ptr) const
{

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
	for_each(m_frames.begin(), m_frames.end(), cu::RemoveRefFonctor<Frame>());
}

int PackAnimation::Layer::
SizeOfUnpackFromBin() const
{

}

int PackAnimation::Layer::
SizeOfPackToBin() const
{

}

void PackAnimation::Layer::
PackToBin(uint8_t** ptr) const
{

}

}