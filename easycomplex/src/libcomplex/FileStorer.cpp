#include "FileStorer.h"

#include "StoreToJson.h"
#include "StoreToBin.h"

#include <gum/FilepathHelper.h>

namespace ecomplex
{

void FileStorer::Store(const std::string& filepath, const Symbol& sym, 
	                   const std::string& dir, bool store_history)
{
	auto ext = gum::FilepathHelper::Extension(filepath);
	if (ext == ".json") {
		StoreToJson::StoreToFile(filepath, sym, dir, store_history);
	} else if (ext == ".bin") {
		StoreToBin::Store(filepath, sym, dir);
	}
}

}