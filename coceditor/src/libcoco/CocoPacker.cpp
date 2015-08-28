#include "CocoPacker.h"
#include "PackAnimation.h"
#include "PackNodeFactory.h"

#include <easycomplex.h>
#include <easyanim.h>

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
	PackNodeFactory* factory = PackNodeFactory::Instance();
	for (int i = 0, n = m_symbols.size(); i < n; ++i) 
	{
		const d2d::ISymbol* symbol = m_symbols[i];
		if (const ecomplex::Symbol* complex = dynamic_cast<const ecomplex::Symbol*>(symbol)) {
			factory->CreateComplex(complex);
		} else if (const libanim::Symbol* anim = dynamic_cast<const libanim::Symbol*>(symbol)) {
			factory->CreateAnim(anim);
		} else {
			throw d2d::Exception("CocoPacker::Pack unhandled type.");
		}
	}

	
}

}