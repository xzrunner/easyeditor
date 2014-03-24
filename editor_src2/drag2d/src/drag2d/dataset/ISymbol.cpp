#include "ISymbol.h"
#include "SymbolMgr.h"

#include "common/FileNameTools.h"

namespace d2d
{

ISymbol::~ISymbol()
{
	SymbolMgr::Instance()->remove(this);
}

bool ISymbol::loadFromFile(const wxString& filepath)
{
	m_filepath = FilenameTools::getExistFilepath(filepath);
	if (m_filepath == wxEmptyString)
	{
		m_name = wxT("error");
		m_bitmap = NULL;
		return false;
	}

	m_name = FilenameTools::getFilename(filepath);
//	m_bitmap = NULL;
	loadResources();
	return true;
}

} // d2d