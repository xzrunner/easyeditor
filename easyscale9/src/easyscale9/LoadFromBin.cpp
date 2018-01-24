#include "LoadFromBin.h"

#include <ee/Config.h>
#include <ee/SettingData.h>
#include <ee/SymbolMgr.h>
#include <ee/SpriteFactory.h>
#include <ee/FileHelper.h>
#include <ee/sprite_msg.h>

#include <sns/NodeFactory.h>
#include <sns/Scale9Sym.h>
#include <sns/NodeSpr.h>
#include <memmgr/LinearAllocator.h>
#include <bs/ImportStream.h>

namespace escale9
{

void LoadFromBin::Load(const std::string& filepath)
{
	mm::LinearAllocator alloc;

	auto sym = sns::NodeFactory::CreateSymFromBin(alloc, filepath);
	auto sym_src = dynamic_cast<sns::Scale9Sym*>(sym);

	ee::SettingData& data = ee::Config::Instance()->GetSettings();
	bool ori_clip_cfg = data.open_image_edge_clip;
	data.open_image_edge_clip = false;

	auto dir = ee::FileHelper::GetFileDir(filepath);
	for (int i = 0; i < 9; ++i) 
	{
		auto child = sym_src->GetChildByIndex(i);
		if (!child) {
			continue;
		}
		auto child_path = ee::FileHelper::GetAbsolutePath(
			dir, child->GetCommon().GetFilepath());
		auto sym = ee::SymbolMgr::Instance()->FetchSymbol(child_path);
		auto spr = ee::SpriteFactory::Instance()->Create(sym);
		spr->Load(child);
		spr->SetVisible(true);

		ee::InsertSpriteSJ::Instance()->Insert(spr);
	}
}

}