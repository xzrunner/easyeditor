#include "FileTools.h"

#include <wx/dir.h>
#include <wx/filename.h>

namespace d2d
{

bool mk_dir(const std::string& dir, bool rm)
{
	bool ret = true;
	if (wxDir::Exists(dir)) {
		if (rm) {
			wxFileName::Rmdir(dir, wxPATH_RMDIR_RECURSIVE);
			ret = wxFileName::Mkdir(dir, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
		}
	} else {
		ret = wxFileName::Mkdir(dir, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
	}
	return ret;
}

}