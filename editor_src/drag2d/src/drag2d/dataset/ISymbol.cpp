#include "ISymbol.h"

#include "common/FileNameTools.h"
#include "common/Context.h"

namespace d2d
{

bool ISymbol::loadFromFile(const wxString& filepath)
{
	m_filepath = FilenameTools::getExistFilepath(filepath);
	if (m_filepath == wxEmptyString)
	{
		m_name = wxT("error");
		m_bitmap = NULL;
		return false;
	}

	Context::Instance()->resPath = FilenameTools::getFileDir(filepath) + "\\";

	m_name = FilenameTools::getFilename(filepath);
//	m_bitmap = NULL;
	loadResources();
	return true;
}

} // d2d