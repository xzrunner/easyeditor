#include "SymbolFactory.h"
#include "ImageSymbol.h"
#include "FontBlankSymbol.h"
#include "ScriptsSymbol.h"
#include "FileHelper.h"
#include "StringHelper.h"
#include "SymbolFile.h"
#include "SymbolType.h"

#include <sprite2/SymType.h>

namespace ee
{

SymbolFactory::CallbackMap SymbolFactory::m_creators;

Symbol* SymbolFactory::Create(const std::string& filepath)
{
	Symbol* sym = NULL;
	int type = ee::SymbolFile::Instance()->Type(filepath);
	switch (type)
	{
	case s2::SYM_IMAGE:
		sym = new ImageSymbol;
		break;
	case ee::SYM_SCRIPTS:
		sym = new ScriptsSymbol;
		break;
	case ee::SYM_FONTBLANK:
		sym = new FontBlankSymbol;
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