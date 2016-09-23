#include "TransOldShapeFile.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>

#include <easyshape.h>
#include <easytexture.h>

namespace edb
{

std::string TransOldShapeFile::Command() const
{
	return "trans-old-shape";
}

std::string TransOldShapeFile::Description() const
{
	return "trans old shape file";
}

std::string TransOldShapeFile::Usage() const
{
	// trans-old-shape e:/test2/test_shape.json
	std::string usage = Command() + " [folder]";
	return usage;
}

int TransOldShapeFile::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	Run(argv[2]);

	return 0;
}

void TransOldShapeFile::Run(const std::string& folder)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(folder, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (ee::FileType::IsType(filepath, ee::FILE_SHAPE)) {
// 			ee::Symbol* sym = ee::SymbolMgr::Instance()->fetchSymbol(filepath);
// 			static_cast<eshape::Symbol*>(sym)->StoreToFile(sym->getFilepath());
		} else if (ee::FileType::IsType(filepath, ee::FILE_TEXTURE)) {
			ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
			etexture::FileSaver::Store(filepath.c_str(), static_cast<etexture::Symbol*>(sym));
		}
	}
}

}