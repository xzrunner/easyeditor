#include "TransToETC1.h"
#include "check_params.h"

#include <easyimage.h>

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
	return Command() + " [dir path]";
}

void TransToETC1::Run(int argc, char *argv[])
{
	// trans2etc1 e:/test2/1001

	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;

	Trigger(argv[2]);
}

void TransToETC1::Trigger(const std::string& dir)
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image))
		{
			std::cout << i << " / " << n << " : " << filepath << "\n";

			int w, h, c, f;
			uint8_t* pixels = eimage::ImageIO::Read(filepath.c_str(), w, h, c, f);
			eimage::TransToETC1 trans(pixels, w, h, c);
//			std::string out_file = filepath.substr(0, filepath.find_last_of('.')) + ".pkm";
			std::string out_file = filepath.substr(0, filepath.find_last_of('.'));
			trans.OutputFile(out_file);
		}
	}
}

}