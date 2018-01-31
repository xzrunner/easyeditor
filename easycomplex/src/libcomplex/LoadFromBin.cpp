#include "LoadFromBin.h"
#include "Symbol.h"

#include <ee/SymbolMgr.h>
#include <ee/SpriteFactory.h>
#include <ee/FileHelper.h>

#include <sns/NodeFactory.h>
#include <sns/NodeSym.h>
#include <sns/NodeSpr.h>
#include <sns/ComplexSym.h>
#include <memmgr/LinearAllocator.h>
#include <bs/ImportStream.h>

#include <memory>
#include <fstream>

namespace ecomplex
{

void LoadFromBin::Load(const std::string& filepath, Symbol& complex)
{
	mm::LinearAllocator alloc;

	auto sym = sns::NodeFactory::CreateSymFromBin(alloc, filepath);
	auto sym_src = dynamic_cast<sns::ComplexSym*>(sym);

	sm::rect scissor;
	scissor.xmin = sym_src->scissor[0];
	scissor.ymin = sym_src->scissor[1];
	scissor.xmax = sym_src->scissor[2];
	scissor.ymax = sym_src->scissor[3];
	complex.SetScissor(scissor);

	auto dir = ee::FileHelper::GetFileDir(filepath);
	for (int i = 0; i < sym_src->children_n; ++i) 
	{
		auto& child = sym_src->children[i];
		auto child_path = ee::FileHelper::GetAbsolutePath(
			dir, child->GetCommon().GetFilepath());
		auto sym = ee::SymbolMgr::Instance()->FetchSymbol(child_path);
		auto spr = ee::SpriteFactory::Instance()->Create(sym);
		spr->Load(child);
		spr->SetVisible(true);
		complex.Add(spr);
	}
}

}