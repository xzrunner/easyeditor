#include "Symbol.h"
#include "SymbolMgr.h"
#include "FileHelper.h"
#include "SymbolType.h"
#include "Bitmap.h"

namespace ee
{

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
	m_bitmap.reset();
	if (m_filepath.empty()) {
		m_filepath = filepath;
	}
	return LoadResources();
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