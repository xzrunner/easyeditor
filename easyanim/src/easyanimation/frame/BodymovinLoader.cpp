#include "BodymovinLoader.h"
#include "message/messages.h"
#include "dataset/DataMgr.h"
#include "dataset/KeyFrame.h"
#include "dataset/Layer.h"
#include "view/ViewMgr.h"
#include "view/ToolbarPanel.h"
#include "view/LibraryPanel.h"

#include <ee/SymbolLoader.h>
#include <ee/SpriteLoader.h>
#include <ee/Sprite.h>
#include <ee/SymbolMgr.h>

#include <gum/BodymovinAnimLoader.h>

namespace eanim
{

void BodymovinLoader::Load(const Json::Value& val, const std::string& dir)
{
	auto sym_loader = std::make_shared<ee::SymbolLoader>();
	auto spr_loader = std::make_shared<ee::SpriteLoader>();
	auto sym = std::make_shared<s2::AnimSymbol>();
	gum::BodymovinAnimLoader loader(*std::dynamic_pointer_cast<s2::AnimSymbol>(sym), sym_loader, spr_loader);
	loader.LoadJson(val, dir.c_str());

	SetFpsSJ::Instance()->Set(sym->GetFPS());

	ViewMgr::Instance()->toolbar->ChangeTemplateMode(true);

	DataMgr::Instance()->name = "";
	
	DataMgr::Instance()->GetLayers().Clear();
	ViewMgr::Instance()->library->Clear();
	SetSelectedSJ::Instance()->Set(-1, -1);

	const auto& layers = sym->GetLayers();
	for (int i = 0, n = layers.size(); i < n; ++i)
	{
		const auto& src_layer = layers[i];
		
		Layer* dst_layer = new Layer;
		InsertLayerSJ::Instance()->Insert(dst_layer);
		SetSelectedSJ::Instance()->Set(0, 0);

		for (int j = 0, m = src_layer->frames.size(); j < m; ++j)
		{
			const auto& src_frame = src_layer->frames[j];
			KeyFrame* dst_frame = new KeyFrame(src_frame->index);
			dst_frame->SetLayer(dst_layer);

			for (auto& spr : src_frame->sprs) {
				dst_frame->Insert(std::dynamic_pointer_cast<ee::Sprite>(spr->Clone()), INT_MAX);
			}

			dst_frame->SetClassicTween(src_frame->tween);

			dst_layer->InsertKeyFrame(dst_frame);
			dst_frame->RemoveReference();
		}
	}

	ViewMgr::Instance()->library->LoadFromSymbolMgr(*ee::SymbolMgr::Instance());

	SetSelectedSJ::Instance()->Set(0, 0);
}

}