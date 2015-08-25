#include "CocoPacker.h"

namespace libcoco
{

void CocoPacker::LoadData(const std::string& dir)
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir, files);

	for (int i = 0, n = files.size(); i < n; ++i) 
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex) || 
			d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim))
		{
			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
			m_symbols.push_back(symbol);
		}
	}
}

void CocoPacker::Pack() const
{
	for (int i = 0, n = m_symbols.size(); i < n; ++i) {
		
	}
}

}