#include "TransOldShapeFile.h"
#include "check_params.h"

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

void TransOldShapeFile::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;

	Run(argv[2]);
}

void TransOldShapeFile::Run(const std::string& folder)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(folder, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath().ToStdString();

		if (ee::FileType::IsType(filepath, ee::FileType::e_shape)) {
// 			ee::Symbol* symbol = ee::SymbolMgr::Instance()->fetchSymbol(filepath);
// 			static_cast<libshape::Symbol*>(symbol)->StoreToFile(symbol->getFilepath());
		} else if (ee::FileType::IsType(filepath, ee::FileType::e_texture)) {
			ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
			etexture::FileSaver::Store(filepath.c_str(), static_cast<etexture::Symbol*>(symbol));
		}
	}
}

}