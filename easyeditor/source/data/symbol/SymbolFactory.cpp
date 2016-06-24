#include "SymbolFactory.h"
#include "ImageSymbol.h"
#include "FontBlankSymbol.h"
#include "ScriptsSymbol.h"
#include "FileHelper.h"
#include "StringHelper.h"

namespace ee
{

SymbolFactory::CallbackMap SymbolFactory::m_creators;

Symbol* SymbolFactory::create(const std::string& filepath)
{
	Symbol* symbol = NULL;

	std::string ext = FileHelper::GetExtension(filepath);
	StringHelper::ToLower(ext);
	if (ext == "png" || ext == "jpg" || ext == "bmp" || ext == "pvr" || ext == "pkm")
	{
		symbol = new ImageSymbol;
	}
	else if (ext == "json")
	{
		std::string type = FileType::GetTag(FileType::GetType(filepath));
		CallbackMap::iterator itr = m_creators.find(type);
		if (itr != m_creators.end()) {
			symbol = (itr->second)();
		}
		else if (FileType::IsType(filepath, FileType::e_fontblank)) {
			symbol = new FontBlankSymbol;
		}
	}
	else if (ext == "lua")
	{
		symbol = new ScriptsSymbol;
	}

	return symbol;
}

void SymbolFactory::RegisterCreator(const std::string& type, CreateCallback cb)
{
	m_creators.insert(std::make_pair(type, cb));
}

void SymbolFactory::UnregisterCreator(const std::string& type)
{
	m_creators.erase(type);
}

}