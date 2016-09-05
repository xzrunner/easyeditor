#ifndef _GLUE_SYMBOL_FILE_TYPE_H_
#define _GLUE_SYMBOL_FILE_TYPE_H_

#include <string>

namespace glue
{

enum SymbolFileType
{
	UNKNOWN		= 0,
	IMAGE,
	SCALE9,
};

SymbolFileType get_sym_file_type(const std::string& filepath);

std::string get_sym_tag_str(SymbolFileType type);

}

#endif // _GLUE_SYMBOL_FILE_TYPE_H_