#ifndef _EASYEDITOR_FILE_TYPE_H_
#define _EASYEDITOR_FILE_TYPE_H_

#include <string>

namespace ee
{

class FileType
{
public:
	enum Type
	{
		e_unknown = 0,
		// shape
		e_shape,
		// symbol
		e_image,
		e_mesh,
		e_combination,
		e_complex,
		e_anim,
		e_anis,
		e_scale9,
		e_fontblank,
		e_freetype,
		e_scripts,
		e_particle2d,
		e_particle3d,
		e_p3dinv,
		e_ejoy2d,
		e_texture,
		e_terrain2d,
		e_icon,
		e_shadow,
		e_ui,
		e_text,
		e_mask,
	};

	static Type GetType(const std::string& filename);
	static std::string GetTag(Type type);

	static bool IsType(const std::string& filename, Type type);

}; // FileType

}

#endif // _EASYEDITOR_FILE_TYPE_H_