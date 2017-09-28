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
	ee::SymbolLoader sym_loader;
	ee::SpriteLoader spr_loader;
	s2::AnimSymbol* sym = new s2::AnimSymbol();
	gum::BodymovinAnimLoader loader(sym, &sym_loader, &spr_loader);
	loader.LoadJson(val, dir);

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

			for (int k = 0, l = src_frame->sprs.size(); k < l; ++k) {
				ee::Sprite* spr = VI_CLONE(ee::Sprite, src_frame->sprs[k]);
				dst_frame->Insert(spr, INT_MAX);
			}

			dst_frame->SetClassicTween(src_frame->tween);

			dst_layer->InsertKeyFrame(dst_frame);
			dst_frame->RemoveReference();
		}
	}

	sym->RemoveReference();

	ViewMgr::Instance()->library->LoadFromSymbolMgr(*ee::SymbolMgr::Instance());

	SetSelectedSJ::Instance()->Set(0, 0);
}

}