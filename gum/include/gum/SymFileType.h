#ifndef _GUM_SYM_FILE_TYPE_H_
#define _GUM_SYM_FILE_TYPE_H_

#include <string>

namespace gum
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
	
	MESH,
	MASK,
	TRAIL,
};

SymFileType get_sym_file_type(const std::string& filepath);

std::string get_sym_tag_str(SymFileType type);

}

#endif // _GUM_SYM_FILE_TYPE_H_
