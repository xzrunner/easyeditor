#include "SymbolFactory.h"
#include "ImageSymbol.h"
#include "ShapeSymbol.h"
#include "EShapeSymbol.h"
#include "MeshSymbol.h"
#include "CombinationSymbol.h"
#include "ComplexSymbol.h"
#include "AnimSymbol.h"
#include "Patch9Symbol.h"
#include "FontSymbol.h"
#include "FontBlankSymbol.h"
#include "ScriptsSymbol.h"

#include "common/FileNameTools.h"
#include "common/FileNameParser.h"

namespace d2d
{

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
	else if (ext == "txt")
	{
		if (FileNameParser::isType(filepath, FileNameParser::e_polygon))
			symbol = new ShapeSymbol;
		else if (FileNameParser::isType(filepath, FileNameParser::e_mesh))
			symbol = new MeshSymbol;
		else if (FileNameParser::isType(filepath, FileNameParser::e_combination))
			symbol = new CombinationSymbol;
	}
	else if (ext == "json")
	{
		if (FileNameParser::isType(filepath, FileNameParser::e_shape))
			symbol = new EShapeSymbol;
		else if (FileNameParser::isType(filepath, FileNameParser::e_complex))
			symbol = new ComplexSymbol;
		else if (FileNameParser::isType(filepath, FileNameParser::e_anim))
			symbol = new AnimSymbol;
		else if (FileNameParser::isType(filepath, FileNameParser::e_9patch))
			symbol = new Patch9Symbol;
		else if (FileNameParser::isType(filepath, FileNameParser::e_fontblank))
			symbol = new FontBlankSymbol;
	}
	else if (ext == "lua")
	{
		symbol = new ScriptsSymbol;
	}

	return symbol;
}

} // d2d