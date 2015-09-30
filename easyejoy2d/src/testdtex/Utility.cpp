#include "Utility.h"

#include <dtex.h>

namespace tdtex
{

dtex_package* Utility::LoadPackage(const std::string& pkg_name, float scale,
								   bool load_texture_immediately/*, bool load_texture_async*/)
{
	dtex_package* pkg = dtexf_preload_pkg(pkg_name.c_str(), (pkg_name + ".epe").c_str(), FILE_EPE, scale);

	int idx = 1;
	while (true)
	{
		std::string path = pkg_name	+ "." + d2d::StringTools::ToString(idx++) + ".ept";
		if (!d2d::FilenameTools::IsFileExist(path)) {
			break;
		}
		dtex_package* _pkg = dtexf_preload_pkg(pkg_name.c_str(), path.c_str(), FILE_EPT, scale);
		assert(_pkg == pkg);
	}

	if (load_texture_immediately) {
		for (int i = 0; i < pkg->texture_count; ++i) {
			dtexf_load_texture(pkg, i, scale);
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