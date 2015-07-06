#include "ISymbol.h"
#include "SymbolMgr.h"

#include "common/FileNameTools.h"

namespace d2d
{

ISymbol::~ISymbol()
{
	SymbolMgr::Instance()->remove(this);
}

void ISymbol::ClearUserData(bool deletePtr)
{
	delete m_userData, m_userData = NULL;
}

bool ISymbol::LoadFromFile(const std::string& filepath)
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
	LoadResources();
	return true;
}

const std::set<std::string>& ISymbol::GetFilepaths() const 
{
	return m_filepaths;
}

void ISymbol::SetFilepaths(const std::set<std::string>& filepaths) 
{
	m_filepaths = filepaths;
}


} // d2d