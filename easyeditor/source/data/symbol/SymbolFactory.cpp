#include "SymbolFactory.h"
#include "ImageSymbol.h"
#include "FontBlankSymbol.h"
#include "ScriptsSymbol.h"
#include "FileHelper.h"
#include "StringHelper.h"
#include "SymbolFile.h"
#include "SymbolType.h"

#include <cu/cu_stl.h>
#include <sprite2/SymType.h>

namespace ee
{

SymbolFactory::CallbackMap SymbolFactory::m_creators;

SymPtr SymbolFactory::Create(const std::string& filepath, int type)
{
	if (type == s2::SYM_UNKNOWN) {
		type = ee::SymbolFile::Instance()->Type(filepath);
	}
	return Create(type);
}

SymPtr SymbolFactory::Create(int type)
{
	SymPtr sym = nullptr;
	switch (type)
	{
	case s2::SYM_IMAGE:
		sym = CU_MAKE_SHARED<ImageSymbol>();
		break;
	case ee::SYM_SCRIPTS:
		sym = CU_MAKE_SHARED<ScriptsSymbol>();
		break;
	case ee::SYM_FONTBLANK:
		sym = CU_MAKE_SHARED<FontBlankSymbol>();
		break;
	default:
		{
			CallbackMap::iterator itr = m_creators.find(type);
			if (itr != m_creators.end()) {
				sym = (itr->second)();
			}
		}
	}
	return sym;
}

void SymbolFactory::RegisterCreator(int type, CreateCallback cb)
{
	m_creators.insert(std::make_pair(type, cb));
}

void SymbolFactory::UnregisterCreator(int type)
{
	m_creators.erase(type);
}

}