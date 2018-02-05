#include "s2loader/EasyAnimLoader.h"
#include "s2loader/SpriteFactory.h"
#include "s2loader/SprTransLoader.h"
#include "s2loader/SpriteLoader.h"

#include <sprite2/Sprite.h>
#include <sprite2/LerpCircle.h>
#include <sprite2/LerpSpiral.h>
#include <sprite2/LerpWiggle.h>
#include <sprite2/LerpEase.h>
#include <sprite2/AnimLerp.h>
#include <bs/FixedPointNum.h>
#include <s2s/NodeSpr.h>

#include <assert.h>

namespace s2loader
{

EasyAnimLoader::EasyAnimLoader(s2::AnimSymbol& sym, 
	                           const std::shared_ptr<const SpriteLoader>& spr_loader)
	: m_sym(sym)
	, m_spr_loader(spr_loader)
{
	if (!m_spr_loader) {
		m_spr_loader = std::make_shared<SpriteLoader>();
	}
}

void EasyAnimLoader::LoadJson(const Json::Value& val, const CU_STR& dir)
{
	int fps = val["fps"].asInt();
	m_sym.SetFPS(fps);

	LoadLayers(val["layer"], dir);
}

void EasyAnimLoader::LoadBin(const simp::NodeAnimation* node)
{
	for (int layer = 0; layer < node->n; ++layer)
	{
		const simp::NodeAnimation::Layer* src_layer = &node->layers[layer];
		auto dst_layer = CU_MAKE_UNIQUE<s2::AnimSymbol::Layer>();
		int frame_n = src_layer->n;
		dst_layer->frames.reserve(frame_n);
		for (int frame = 0; frame < frame_n; ++frame)
		{
			const simp::NodeAnimation::Frame* src_frame = src_layer->frames[frame];
			auto dst_frame = CU_MAKE_UNIQUE<s2::AnimSymbol::Frame>();
			dst_frame->index = src_frame->index;
			dst_frame->tween = bs::int2bool(src_frame->tween);
			LoadActors(src_frame, *dst_frame);
			LoadLerps(src_frame, *dst_frame);
			dst_layer->frames.push_back(std::move(dst_frame));
		}
		m_sym.AddLayer(dst_layer);
	}
}

void EasyAnimLoader::LoadSns(const s2s::AnimSym* sym, const CU_STR& dir)
{
	for (int layer = 0; layer < sym->m_layers_n; ++layer)
	{
		auto& src_layer = sym->m_layers[layer];
		auto dst_layer = CU_MAKE_UNIQUE<s2::AnimSymbol::Layer>();
		dst_layer->frames.reserve(src_layer.frames_n);
		for (int frame = 0; frame < src_layer.frames_n; ++frame)
		{
			auto& src_frame = src_layer.frames[frame];
			auto dst_frame = CU_MAKE_UNIQUE<s2::AnimSymbol::Frame>();
			dst_frame->index = src_frame.index;
			dst_frame->tween = bs::int2bool(src_frame.tween);
			LoadActors(src_frame, *dst_frame, dir);
			LoadLerps(src_frame, *dst_frame);
			dst_layer->frames.push_back(std::move(dst_frame));
		}
		m_sym.AddLayer(dst_layer);
	}
}

void EasyAnimLoader::LoadLayers(const Json::Value& val, const CU_STR& dir)
{
	int layer_n = val.size();
	for (int layer = 0; layer < layer_n; ++layer)
	{
		const Json::Value& layer_val = val[layer];
		auto dst_layer = CU_MAKE_UNIQUE<s2::AnimSymbol::Layer>();
		dst_layer->name = layer_val["name"].asString().c_str();
		int frame_n = layer_val["frame"].size();
		dst_layer->frames.reserve(frame_n);
		for (int frame = 0; frame < frame_n; ++frame)
		{
			const Json::Value& frame_val = layer_val["frame"][frame];
			auto dst_frame = CU_MAKE_UNIQUE<s2::AnimSymbol::Frame>();
			dst_frame->index = frame_val["time"].asInt();
			dst_frame->tween = frame_val["tween"].asBool();
			LoadActors(frame_val, *dst_frame, dir);
			LoadLerps(frame_val, *dst_frame);
			dst_layer->frames.push_back(std::move(dst_frame));
		}
		m_sym.AddLayer(dst_layer);
	}
}

void EasyAnimLoader::LoadActors(const Json::Value& src, s2::AnimSymbol::Frame& dst,
								const CU_STR& dir)
{
	int actor_n = src["actor"].size();
	dst.sprs.reserve(actor_n);
	for (int actor = 0; actor < actor_n; ++actor)
	{
		const Json::Value& actor_val = src["actor"][actor];
		auto spr = m_spr_loader->Create(actor_val, dir);
		dst.sprs.push_back(spr);
	}
}

void EasyAnimLoader::LoadLerps(const Json::Value& src, s2::AnimSymbol::Frame& dst)
{
	for (int i = 0, n = src["lerp"].size(); i < n; ++i)
	{
		std::unique_ptr<s2::ILerp> lerp = nullptr;
		const Json::Value& val = src["lerp"][i]["val"];
		CU_STR type = val["type"].asString().c_str();
		if (type == "circle")
		{
			float scale = val["scale"].asInt() * 0.01f;
			lerp = std::make_unique<s2::LerpCircle>(scale);
		}
		else if (type == "spiral") 
		{
			float begin = val["angle_begin"].asInt() * SM_DEG_TO_RAD,
				  end   = val["angle_end"].asInt() * SM_DEG_TO_RAD;
			float scale = val["scale"].asInt() * 0.01f;
			lerp = std::make_unique<s2::LerpSpiral>(begin, end, scale);
		}
		else if (type == "wiggle")
		{
			float freq = static_cast<float>(val["freq"].asDouble());
			float amp = static_cast<float>(val["amp"].asDouble());
			lerp = std::make_unique<s2::LerpWiggle>(freq, amp);
		}
		else if (type == "ease")
		{
			int type = val["ease"].asInt();
			lerp = std::make_unique<s2::LerpEase>(type);
		}
		if (lerp) {
			s2::AnimLerp::SprData key = s2::AnimLerp::SprData(src["lerp"][i]["key"].asInt());
			dst.lerps.push_back(std::make_pair(key, std::move(lerp)));
		}
	}
}

void EasyAnimLoader::LoadActors(const simp::NodeAnimation::Frame* src, s2::AnimSymbol::Frame& dst)
{
	dst.sprs.reserve(src->actors_n);
	for (int i = 0; i < src->actors_n; ++i)
	{
		const simp::NodeAnimation::Actor* src_actor = src->actors[i];
		auto spr = SpriteFactory::Instance()->Create(src_actor->sym_id);
		if (spr) {
			SprTransLoader::Load(spr, src_actor->trans);
			dst.sprs.push_back(spr);
		}
	}
}

void EasyAnimLoader::LoadLerps(const simp::NodeAnimation::Frame* src, s2::AnimSymbol::Frame& dst)
{
	dst.lerps.reserve(src->lerps_n);
	for (int i = 0; i < src->lerps_n; ++i)
	{
		const simp::NodeAnimation::Lerp* s = src->lerps[i];
		std::unique_ptr<s2::ILerp> lerp = nullptr;
		switch (s->type)
		{
		case s2::LERP_CIRCLE:
			{
				assert(s->data_n == 1);
				float scale = 0;
				memcpy(&scale, &s->data[0], sizeof(float));
				lerp = std::make_unique<s2::LerpCircle>(scale);
				break;
			}
		case s2::LERP_SPIRAL:
			{
				assert(s->data_n == 3);
				float begin = 0, end = 0;
				float scale = 0;
				memcpy(&begin, &s->data[0], sizeof(float));
				memcpy(&end, &s->data[1], sizeof(float));
				memcpy(&scale, &s->data[2], sizeof(float));
				lerp = std::make_unique<s2::LerpSpiral>(begin, end, scale);
				break;
			}
		case s2::LERP_WIGGLE:
			{
				assert(s->data_n == 2);
				float freq = 0, amp = 0;
				memcpy(&freq, &s->data[0], sizeof(float));
				memcpy(&amp, &s->data[1], sizeof(float));
				lerp = std::make_unique<s2::LerpWiggle>(freq, amp);
				break;
			}
		case s2::LERP_EASE:
			{
				assert(s->data_n == 1);
				lerp = std::make_unique<s2::LerpEase>(s->data[0]);
				break;
			}
		}
		if (lerp) {
			s2::AnimLerp::SprData key = static_cast<s2::AnimLerp::SprData>(s->spr_data);
			dst.lerps.push_back(std::make_pair(key, std::move(lerp)));
		}
	}
}

void EasyAnimLoader::LoadActors(const s2s::AnimSym::Frame& src, s2::AnimSymbol::Frame& dst, const CU_STR& dir)
{
	dst.sprs.reserve(src.actors_n);
	for (int i = 0; i < src.actors_n; ++i)
	{
		auto src_actor = src.actors[i];
		auto spr = m_spr_loader->Create(src_actor, dir);
		dst.sprs.push_back(spr);
	}
}

void EasyAnimLoader::LoadLerps(const s2s::AnimSym::Frame& src, s2::AnimSymbol::Frame& dst)
{
	dst.lerps.reserve(src.lerps_n);
	for (int i = 0; i < src.lerps_n; ++i)
	{
		auto s = src.lerps[i];
		std::unique_ptr<s2::ILerp> lerp = nullptr;
		switch (s->type)
		{
		case s2::LERP_CIRCLE:
			{
				assert(s->data_n == 1);
				float scale = 0;
				memcpy(&scale, &s->data[0], sizeof(float));
				lerp = std::make_unique<s2::LerpCircle>(scale);
				break;
			}
		case s2::LERP_SPIRAL:
			{
				assert(s->data_n == 3);
				float begin = 0, end = 0;
				float scale = 0;
				memcpy(&begin, &s->data[0], sizeof(float));
				memcpy(&end, &s->data[1], sizeof(float));
				memcpy(&scale, &s->data[2], sizeof(float));
				lerp = std::make_unique<s2::LerpSpiral>(begin, end, scale);
				break;
			}
		case s2::LERP_WIGGLE:
			{
				assert(s->data_n == 2);
				float freq = 0, amp = 0;
				memcpy(&freq, &s->data[0], sizeof(float));
				memcpy(&amp, &s->data[1], sizeof(float));
				lerp = std::make_unique<s2::LerpWiggle>(freq, amp);
				break;
			}
		case s2::LERP_EASE:
			{
				assert(s->data_n == 1);
				lerp = std::make_unique<s2::LerpEase>(s->data[0]);
				break;
			}
		}
		if (lerp) {
			s2::AnimLerp::SprData key = static_cast<s2::AnimLerp::SprData>(s->spr_data);
			dst.lerps.push_back(std::make_pair(key, std::move(lerp)));
		}
	}
}

}