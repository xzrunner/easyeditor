#include "SymbolFactory.h"
#include "ImageSymbol.h"
#include "FontBlankSymbol.h"
#include "ScriptsSymbol.h"

namespace ee
{

SymbolFactory::CallbackMap SymbolFactory::m_creators;

Symbol* SymbolFactory::create(const wxString& filepath)
{
	Symbol* symbol = NULL;

	wxString ext = FileHelper::GetExtension(filepath).Lower();

	if (ext == "png" || ext == "jpg" || ext == "bmp" || ext == "pvr")
	{
		symbol = new ImageSymbol;
	}
	else if (ext == "json")
	{
		wxString type = FileType::GetTag(FileType::GetType(filepath));
		CallbackMap::iterator itr = m_creators.find(type.ToStdString());
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