#include "SymbolPath.h"
#include "Symbol.h"
#include "SymbolType.h"
#include "FileHelper.h"

namespace ee
{

std::string SymbolPath::GetRelativePath(const Symbol* sym, const std::string& dir)
{
	const std::string& filepath = sym->GetFilepath();
	if (filepath == SYM_SHAPE_TAG ||
		filepath == SYM_TEXT_TAG) {
		return filepath;
	}

	return FileHelper::GetRelativePath(dir, filepath);
}

}