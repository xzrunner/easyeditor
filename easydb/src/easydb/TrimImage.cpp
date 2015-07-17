#include "TrimImage.h"
#include "check_params.h"

#include <drag2d.h>
#include <easyimage.h>

namespace edb
{

static const char* OUTPUT_FILE = "trim";

std::string TrimImage::Command() const
{
	return "trim-image";
}

std::string TrimImage::Description() const
{
	return "trim image, clip blank part";
}

std::string TrimImage::Usage() const
{
	return Command() + " [dir path]";
}

void TrimImage::Run(int argc, char *argv[])
{
	// trim-image e:/test2/1001

	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;

	Trigger(argv[2]);
}

void TrimImage::Trigger(const std::string& dir)
{
	Json::Value value;
	int idx = 0;

	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath().ToStdString();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image))
		{
			std::cout << i << " / " << n << " : " << filepath << "\n";

			d2d::ImageData* img = d2d::ImageDataMgr::Instance()->GetItem(filepath);		

			eimage::ImageTrim trim(*img);
			d2d::Rect r = trim.Trim();

			// save info
			Json::Value spr_val;
			spr_val["filepath"] = d2d::FilenameTools::getRelativePath(dir, filepath).ToStdString();
			spr_val["source size"]["w"] = img->GetWidth();
			spr_val["source size"]["h"] = img->GetHeight();
			spr_val["position"]["x"] = r.xMin;
			spr_val["position"]["y"] = img->GetHeight() - r.yMax;
			spr_val["position"]["w"] = r.xLength();
			spr_val["position"]["h"] = r.yLength();
			value[idx++] = spr_val;

			eimage::ImageClip clip(*img);
			const uint8_t* pixels = clip.Clip(r.xMin, r.xMax, r.yMin, r.yMax);
			d2d::ImageSaver::storeToFile(pixels, r.xLength(), r.yLength(), img->GetChannels(), 
				filepath, d2d::ImageSaver::e_png);
			delete[] pixels;

			img->Release();
		}
	}

	std::string output_file = dir + "\\" + OUTPUT_FILE + ".json";
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(output_file.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

}