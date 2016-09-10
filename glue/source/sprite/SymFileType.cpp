#include "SymFileType.h"

namespace glue
{

static const std::string TAG_SCALE9		= "scale9";
static const std::string TAG_TEXTBOX	= "text";
static const std::string TAG_COMPLEX	= "complex";
static const std::string TAG_ANIM		= "anim";
static const std::string TAG_PARTICLE3D	= "particle";
static const std::string TAG_PARTICLE2D	= "particle2d";

SymFileType get_sym_file_type(const std::string& filepath)
{
	if (filepath.empty()) {
		return UNKNOWN;
	}

	int pos = filepath.rfind('.');
	if (pos == -1) {
		return UNKNOWN;
	}

	std::string ext = filepath.substr(pos + 1);
	
	if (ext == "png" || ext == "jpg" || ext == "bmp" || ext == "pvr" || ext == "pkm") 
	{
		return IMAGE;
	}
	else if (ext == "json") 
	{
		const std::string filename = filepath.substr(0, filepath.find_last_of('.'));
		int pos = filename.find_last_of('_');
		if (pos == -1) {
			return UNKNOWN;
		}

		std::string tag = filename.substr(pos + 1);
		if (tag == TAG_SCALE9) {
			return SCALE9;
		} else if (tag == TAG_TEXTBOX) {
			return TEXTBOX;
		} else if (tag == TAG_COMPLEX) {
			return COMPLEX;
		} else if (tag == TAG_ANIM) {
			return ANIMATION;
		} else if (tag == TAG_PARTICLE3D) {
			return PARTICLE3D;
		} else if (tag == TAG_PARTICLE2D) {
			return PARTICLE2D;
		}
	}

	return UNKNOWN;
}

std::string get_sym_tag_str(SymFileType type)
{
	switch (type)
	{
	case SCALE9:
		return TAG_SCALE9;
	case TEXTBOX:
		return TAG_TEXTBOX;
	case COMPLEX:
		return TAG_COMPLEX;
	case ANIMATION:
		return TAG_ANIM;
	case PARTICLE3D:
		return TAG_PARTICLE3D;
	case PARTICLE2D:
		return TAG_PARTICLE2D;
	}

	return "";
}

}