#include "LayersLoader.h"

#include <ee/SymbolSearcher.h>
#include <ee/SymbolMgr.h>
#include <ee/Exception.h>
#include <ee/SpriteFactory.h>
#include <ee/Sprite.h>

namespace eanim
{

void LayersLoader::LoadLayers(const Json::Value& value, 
							  const std::string& dir, 
							  std::vector<s2::AnimSymbol::Layer*>& layers) const
{
	int i = 0;
	Json::Value layer_val = value["layer"][i++];
	while (!layer_val.isNull()) {
		s2::AnimSymbol::Layer* dst_layer = new s2::AnimSymbol::Layer;
		dst_layer->name = layer_val["name"].asString();
		// frames
		int j = 0;
		Json::Value frame_val = layer_val["frame"][j++];
		while (!frame_val.isNull()) {
			s2::AnimSymbol::Frame* dst_frame = new s2::AnimSymbol::Frame;
			//dstFrame->id = frameValue["id"].asInt();
			dst_frame->index = frame_val["time"].asInt();
			dst_frame->tween = frame_val["tween"].asBool();
			// sprs
			int k = 0;
			Json::Value spr_val = frame_val["actor"][k++];
			while (!spr_val.isNull()) {
				std::string filepath = ee::SymbolSearcher::GetSymbolPath(dir, spr_val);
				ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
				if (!sym) {
					std::string filepath = spr_val["filepath"].asString();
					throw ee::Exception("Symbol doesn't exist, [dir]:%s, [file]:%s !", 
						dir.c_str(), filepath.c_str());
				}
				ee::SymbolSearcher::SetSymbolFilepaths(dir, sym, spr_val);

				ee::Sprite* spr = ee::SpriteFactory::Instance()->Create(sym);
				sym->RemoveReference();
				spr->Load(spr_val, dir);
				dst_frame->sprs.push_back(spr);
				spr_val = frame_val["actor"][k++];
			}
			dst_layer->frames.push_back(dst_frame);
			frame_val = layer_val["frame"][j++];
		}
		layers.push_back(dst_layer);
		layer_val = value["layer"][i++];
	}
}

}