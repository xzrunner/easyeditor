#ifndef _GLUE_SYM_FILE_TYPE_H_
#define _GLUE_SYM_FILE_TYPE_H_

#include <string>

namespace glue
{

enum SymFileType
{
	UNKNOWN		= 0,
	IMAGE,
	SCALE9,
	TEXTBOX,
	COMPLEX,
	ANIMATION,
	PARTICLE3D,
	PARTICLE2D,

	MASK,
	TRAIL,
};

SymFileType get_sym_file_type(const std::string& filepath);

std::string get_sym_tag_str(SymFileType type);

}

#endif // _GLUE_SYM_FILE_TYPE_H_