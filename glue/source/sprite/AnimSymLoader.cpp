#include "AnimSymLoader.h"
#include "FilepathHelper.h"
#include "SpriteFactory.h"

#include <sprite2/AnimSymbol.h>
#include <sprite2/S2_Sprite.h>

#include <fstream>

namespace glue
{

AnimSymLoader::AnimSymLoader(s2::AnimSymbol* sym)
	: m_sym(sym)
{
	if (m_sym) {
		m_sym->AddReference();
	}
}

AnimSymLoader::~AnimSymLoader()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}
}

void AnimSymLoader::LoadJson(const std::string& filepath)
{
	if (!m_sym) {
		return;
	}

	Json::Value val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	std::string dir = FilepathHelper::Dir(filepath);

	int fps = val["fps"].asInt();
	m_sym->SetFPS(fps);

	LoadLayers(val, dir);
}

void AnimSymLoader::LoadLayers(const Json::Value& value, const std::string& dir)
{
	int layer_n = value["layer"].size();
	for (int layer = 0; layer < layer_n; ++layer)
	{
		const Json::Value& layer_val = value["layer"][layer];
		s2::AnimSymbol::Layer* dst_layer = new s2::AnimSymbol::Layer;
		dst_layer->name = layer_val["name"].asString();
		int frame_n = layer_val["frame"].size();
		dst_layer->frames.reserve(frame_n);
		for (int frame = 0; frame < frame_n; ++frame)
		{
			const Json::Value& frame_val = layer_val["frame"][frame];
			s2::AnimSymbol::Frame* dst_frame = new s2::AnimSymbol::Frame;
			dst_frame->index = frame_val["time"].asInt();
			dst_frame->tween = frame_val["tween"].asBool();
			int actor_n = frame_val["actor"].size();
			dst_frame->sprs.reserve(actor_n);
			for (int actor = 0; actor < actor_n; ++actor)
			{
				const Json::Value& actor_val = frame_val["actor"][actor];
				s2::Sprite* spr = LoadSprite(actor_val, dir);
				dst_frame->sprs.push_back(spr);
			}
			dst_layer->frames.push_back(dst_frame);
		}
		m_sym->AddLayer(dst_layer);
	}
}

s2::Sprite* AnimSymLoader::LoadSprite(const Json::Value& val, const std::string& dir)
{
	return SpriteFactory::Instance()->Create(val, dir);
}

}