#include "SymbolFactory.h"
#include "ImageSymbol.h"
#include "FontSymbol.h"
#include "FontBlankSymbol.h"
#include "ScriptsSymbol.h"

#include "common/FileNameTools.h"
#include "common/FileNameParser.h"

#include <easycomplex.h>
#include <easyanim.h>
#include <easymesh.h>
#include <easyscale9.h>

namespace d2d
{

SymbolFactory::CallbackMap SymbolFactory::m_creators;

ISymbol* SymbolFactory::create(const wxString& filepath)
{
	ISymbol* symbol = NULL;

	wxString ext = FilenameTools::getExtension(filepath).Lower();

	if (ext == "png" || ext == "jpg" || ext == "bmp")
	{
		symbol = new ImageSymbol;
	}
	else if (ext == "ttf")
	{
		symbol = new FontSymbol;
	}
	else if (ext == "json")
	{
		size_t s = filepath.find_last_of('_');
		size_t e = filepath.find_last_of('.');
		wxString type = filepath.substr(s+1, e-s-1);
		CallbackMap::iterator itr = m_creators.find(type.ToStdString());
		if (itr != m_creators.end()) {
			symbol = (itr->second)();
		}

// 		else if (FileNameParser::isType(filepath, FileNameParser::e_shape))
// 			symbol = new EShapeSymbol;
//  	else if (FileNameParser::isType(filepath, FileNameParser::e_complex))
// 			symbol = new ecomplex::Symbol;
// 		else if (FileNameParser::isType(filepath, FileNameParser::e_anim))
// 			symbol = new anim::Symbol;
// 		else if (FileNameParser::isType(filepath, FileNameParser::e_scale9))
// 			symbol = new escale9::Symbol;
		else if (FileNameParser::isType(filepath, FileNameParser::e_fontblank))
			symbol = new FontBlankSymbol;
// 		else if (FileNameParser::isType(filepath, FileNameParser::e_mesh))
// 			symbol = new emesh::Symbol;
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