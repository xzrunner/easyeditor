#include "Symbol.h"
#include "SymbolMgr.h"
#include "FileHelper.h"
#include "SymbolType.h"

namespace ee
{

Symbol::~Symbol()
{
	SymbolMgr::Instance()->Remove(this);
}

void Symbol::ClearUserData(bool deletePtr)
{
	delete m_ud, m_ud = NULL;
}

bool Symbol::LoadFromFile(const std::string& filepath)
{
	m_name = FileHelper::GetFilename(filepath);
	if (filepath != SYM_GROUP_TAG) {
		m_filepath = FileHelper::GetExistFilepath(filepath);
	}
	m_bitmap = NULL;
	if (m_filepath == wxEmptyString) {
		m_filepath = filepath;
	}
	LoadResources();
	return true;
}

const std::set<std::string>& Symbol::GetFilepaths() const 
{
	return m_filepaths;
}

void Symbol::SetFilepaths(const std::set<std::string>& filepaths) 
{
	m_filepaths = filepaths;
}


}