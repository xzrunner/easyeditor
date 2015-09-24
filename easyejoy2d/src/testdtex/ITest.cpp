#include "ITest.h"

#include <dtex.h>

namespace tdtex
{

dtex_package* ITest::LoadPackage(const std::string& pkg_name, bool load_tex)
{
	dtex_package* pkg = dtexf_preload_pkg(pkg_name.c_str(), (pkg_name + ".epe").c_str(), FILE_EPE);

	int idx = 1;
	while (true)
	{
		std::string path = pkg_name	+ "." + d2d::StringTools::ToString(idx++) + ".ept";
		if (!d2d::FilenameTools::IsFileExist(path)) {
			break;
		}
		dtex_package* _pkg = dtexf_preload_pkg(pkg_name.c_str(), path.c_str(), FILE_EPT);
		assert(_pkg == pkg);
	}

	if (load_tex) {
		for (int i = 0; i < pkg->tex_size; ++i) {
			dtexf_load_texture(pkg, i);
		}
	}

	return pkg;
}

}