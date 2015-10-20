#include "Utility.h"

#include <dtex.h>

namespace tdtex
{

dtex_package* Utility::LoadPackage(const std::string& pkg_name, float scale,
								   bool load_tex, int lod/*, bool load_texture_async*/)
{
	dtex_package* pkg = dtexf_load_pkg(pkg_name.c_str(), pkg_name.c_str(), FILE_EPE, scale, lod, 0);

	int idx = 1;
	while (true)
	{
		std::string path;
		if (lod == 1) {
			path = pkg_name	+ "." + d2d::StringTools::ToString(idx) + ".50.ept";
		} else if (lod == 2) {
			path = pkg_name	+ "." + d2d::StringTools::ToString(idx) + ".25.ept";
		} else {
			path = pkg_name	+ "." + d2d::StringTools::ToString(idx) + ".ept";
		}
		if (!d2d::FilenameTools::IsFileExist(path)) {
			break;
		}
		dtexf_preload_texture(pkg, idx - 1, scale);
		++idx;
	}

	if (load_tex) {
		for (int i = 0; i < pkg->texture_count; ++i) {
			dtexf_load_texture(pkg, i, false);
		}
	}

// 	if (load_texture_async) {
// 		for (int i = 0; i < pkg->tex_size; ++i) {
// 			std::string filepath = pkg_name	+ "." + d2d::StringTools::ToString(i+1) + ".ept";
// 			dtex_async_load_file(filepath.c_str(), FILE_EPT, pkg, i, scale);
// 		}
// 	} 

	return pkg;
}

}