#pragma once

#include <wx/wx.h>

namespace d2d
{
	class FileNameParser
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
			e_scale9,
			e_fontblank,
			e_freetype,
			e_scripts,
			e_particle2d,
			e_particle3d,
			e_ejoy2d,
			e_texture,
			e_terrain2d,
		};

		static Type getFileType(const wxString& filename);
		static wxString getFileTag(Type type);

		static bool isType(const wxString& filename, Type type);

	}; // FileNameParser
}

