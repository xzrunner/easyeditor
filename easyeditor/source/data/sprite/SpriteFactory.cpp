#include "SpriteFactory.h"
#include "SpritePool.h"
#include "FileHelper.h"
#include "SymbolFile.h"
#include "ImageSprite.h"
#include "ImageSymbol.h"
#include "FontBlankSprite.h"
#include "FontBlankSymbol.h"
#include "ScriptsSprite.h"
#include "ScriptsSymbol.h"
#include "StringHelper.h"
#include "SymbolSearcher.h"
#include "SymbolMgr.h"
#include "Exception.h"
#include "SymbolType.h"

#include <sprite2/SymType.h>
#include <sprite2/ResetActorFlagVisitor.h>
#include <sprite2/CreateActorsVisitor.h>
#include <sprite2/SprVisitorParams.h>
#include <gum/SymbolFile.h>

namespace ee
{

CU_SINGLETON_DEFINITION(SpriteFactory);

SpriteFactory::CallbackMap SpriteFactory::m_creators;

SpriteFactory::SpriteFactory()
	: m_id(0)
{
}

SprPtr SpriteFactory::Create(const SymPtr& sym)
{
	if (!sym) {
		return NULL;
	}

	SprPtr spr = NULL;
	int type = sym->Type();
	switch (type)
	{
	case s2::SYM_IMAGE:
		spr = CU_MAKE_SHARED<ImageSprite>(sym);
		break;
	case SYM_FONTBLANK:
		spr = CU_MAKE_SHARED<FontBlankSprite>(sym);
		break;
	case SYM_SCRIPTS:
		spr = CU_MAKE_SHARED<ScriptsSprite>(sym);
		break;
	default:
		{
			CallbackMap::iterator itr = m_creators.find(type);
			if (itr != m_creators.end()) {
				spr = (itr->second)(sym);
			}
		}
	}

	if (spr) {
		SpritePool::Instance()->Insert(*spr);
		spr->SetName(NextName().c_str());
	}

	if (!sym->tag.empty()) {
		spr->SetTag("[symbol]");
	}

	return spr;
}

SprPtr SpriteFactory::Create(const Json::Value& val, const std::string& dir)
{
	std::string filepath = SymbolSearcher::GetSymbolPath(dir, val);
	return Create(val, dir, filepath);
}

SprPtr SpriteFactory::Create(const Json::Value& val, const std::string& dir, const std::string& filepath)
{
	SymPtr sym = nullptr;
	try {
		sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	} catch (ee::Exception& e) {
		if (val.isMember(gum::SymbolFile::Instance()->Tag(s2::SYM_SKELETON).c_str())) {
			sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath, s2::SYM_SKELETON);
		} else {
			throw e;
		}
	}

	if (!sym) {
		return nullptr;
	}

	SymbolSearcher::SetSymbolFilepaths(dir, *sym, val);
	auto spr = SpriteFactory::Instance()->Create(sym);
	spr->Load(val, dir);
	spr->SetVisible(true);
	return spr;
}

SprPtr SpriteFactory::CreateRoot(const SymPtr& sym)
{
	SprPtr spr = Create(sym);
	if (!spr) {
		return spr;
	}

	s2::ResetActorFlagVisitor v0;
	spr->Traverse(v0, s2::SprVisitorParams());

	s2::CreateActorsVisitor v1;
	spr->Traverse(v1, s2::SprVisitorParams());

	return spr;
}

std::string SpriteFactory::NextName() const
{
	return "_sprite" + StringHelper::ToString(m_id++);
}

void SpriteFactory::RegisterCreator(int type, CreateCallback cb)
{
	m_creators.insert(std::make_pair(type, cb));
}

void SpriteFactory::UnregisterCreator(int type)
{
	m_creators.erase(type);
}

}