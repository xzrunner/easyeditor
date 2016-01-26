#include "SymbolFactory.h"
#include "ImageSymbol.h"
#include "FontBlankSymbol.h"
#include "ScriptsSymbol.h"

#include "common/FileNameTools.h"
#include "common/FileNameParser.h"

namespace d2d
{

SymbolFactory::CallbackMap SymbolFactory::m_creators;

ISymbol* SymbolFactory::create(const wxString& filepath)
{
	ISymbol* symbol = NULL;

	wxString ext = FilenameTools::getExtension(filepath).Lower();

	if (ext == "png" || ext == "jpg" || ext == "bmp" || ext == "pvr")
	{
		symbol = new ImageSymbol;
	}
	else if (ext == "json")
	{
		wxString type = d2d::FileNameParser::getFileTag(d2d::FileNameParser::getFileType(filepath));
		CallbackMap::iterator itr = m_creators.find(type.ToStdString());
		if (itr != m_creators.end()) {
			symbol = (itr->second)();
		}
		else if (FileNameParser::isType(filepath, FileNameParser::e_fontblank)) {
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

} // d2d