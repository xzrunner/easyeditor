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
							  std::vector<Symbol::Layer*>& layers) const
{
	int i = 0;
	Json::Value layer_val = value["layer"][i++];
	while (!layer_val.isNull()) {
		Symbol::Layer* dst_layer = new Symbol::Layer;
		dst_layer->name = layer_val["name"].asString();
		// frames
		int j = 0;
		Json::Value frame_val = layer_val["frame"][j++];
		while (!frame_val.isNull()) {
			Symbol::Frame* dst_frame = new Symbol::Frame;
			//dstFrame->id = frameValue["id"].asInt();
			dst_frame->index = frame_val["time"].asInt();
			dst_frame->bClassicTween = frame_val["tween"].asBool();
			// sprites
			int k = 0;
			Json::Value spr_val = frame_val["actor"][k++];
			while (!spr_val.isNull()) {
				std::string filepath = ee::SymbolSearcher::GetSymbolPath(dir, spr_val);
				ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
				if (!symbol) {
					std::string filepath = spr_val["filepath"].asString();
					throw ee::Exception("Symbol doesn't exist, [dir]:%s, [file]:%s !", 
						dir.c_str(), filepath.c_str());
				}
				ee::SymbolSearcher::SetSymbolFilepaths(dir, symbol, spr_val);

				ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);
				symbol->Release();
				sprite->Load(spr_val);
				dst_frame->sprites.push_back(sprite);
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