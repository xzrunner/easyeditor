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
	int16_t xmin, ymin, xmax, ymax;
	sym_src->GetScissor(xmin, ymin, xmax, ymax);
	complex.SetScissor(scissor);

	auto dir = ee::FileHelper::GetFileDir(filepath);
	for (int i = 0, n = sym_src->GetChildrenNum(); i < n; ++i) 
	{
		auto child = sym_src->GetChildByIndex(i);
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