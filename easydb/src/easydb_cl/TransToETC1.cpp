#include "TransToETC1.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/LibpngAdapter.h>
#include <ee/SymbolFile.h>

#include <easyimage.h>

#include <sprite2/SymType.h>

namespace edb
{

std::string TransToETC1::Command() const
{
	return "trans2etc1";
}

std::string TransToETC1::Description() const
{
	return "trans images to etc1";
}

std::string TransToETC1::Usage() const
{
	return Command() + " [path]";
}

int TransToETC1::Run(int argc, char *argv[])
{
	// trans2etc1 e:/test2/1001

	if (!check_number(this, argc, 3)) return -1;

	Trigger(argv[2]);

	return 0;
}

void TransToETC1::Trigger(const std::string& path)
{
	if (ee::FileHelper::IsDirExist(path)) {
		wxArrayString files;
		ee::FileHelper::FetchAllFiles(path, files);
		for (int i = 0, n = files.size(); i < n; ++i) {
			std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
			std::cout << i << " / " << n << " : " << filepath << "\n";
			Format(files[i].ToStdString());
		}
	} else if (ee::FileHelper::IsFileExist(path)) {
		Format(path);
	}
}

void TransToETC1::Format(const std::string& filepath)
{
	if (ee::SymbolFile::Instance()->Type(filepath) != s2::SYM_IMAGE) {
		return;		
	}

	int w, h, c, f;
	uint8_t* pixels = ee::LibpngAdapter::Read(filepath.c_str(), w, h, c, f);
	eimage::TransToETC1 trans(pixels, w, h, c, false, true);
	//std::string out_file = filepath.substr(0, filepath.find_last_of('.')) + ".pkm";
	std::string out_file = filepath.substr(0, filepath.find_last_of('.'));
	trans.OutputFile(out_file);
}

}