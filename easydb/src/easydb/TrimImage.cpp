#include "TrimImage.h"
#include "check_params.h"

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
			if (!r.isValid()) {
				r.xMin = r.yMin = 0;
				r.xMax = img->GetWidth();
				r.yMax = img->GetHeight();
			}

			// save info
			Json::Value spr_val;
			spr_val["filepath"] = d2d::FilenameTools::getRelativePath(dir, filepath).ToStdString();
			spr_val["source size"]["w"] = img->GetWidth();
			spr_val["source size"]["h"] = img->GetHeight();
			spr_val["position"]["x"] = r.xMin;
			spr_val["position"]["y"] = img->GetHeight() - r.yMax;
			spr_val["position"]["w"] = r.xLength();
			spr_val["position"]["h"] = r.yLength();
			StoreBoundInfo(*img, r, spr_val);
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

void TrimImage::StoreBoundInfo(const d2d::ImageData& img, const d2d::Rect& r, 
							   Json::Value& val) const
{
	// left
	{
		int tot = 0, max = 0;
		int curr = 0;
		for (int y = 0; y < img.GetHeight(); ++y) {
			if (!IsTransparent(img, r.xMin, y)) {
				++curr;
				++tot;
				if (curr > max) {
					max = curr;
				} else {
					curr = 0;
				}
			}
		}
		val["bound"]["left"]["tot"] = tot;
		val["bound"]["left"]["max"] = max;
	}
	// right
	{
		int tot = 0, max = 0;
		int curr = 0;
		for (int y = 0; y < img.GetHeight(); ++y) {
			if (!IsTransparent(img, r.xMax - 1, y)) {
				++curr;
				++tot;
				if (curr > max) {
					max = curr;
				} else {
					curr = 0;
				}
			}
		}
		val["bound"]["right"]["tot"] = tot;
		val["bound"]["right"]["max"] = max;
	}
	// bottom
	{
		int tot = 0, max = 0;
		int curr = 0;
		for (int x = 0; x < img.GetWidth(); ++x) {
			if (!IsTransparent(img, x, r.yMin)) {
				++curr;
				++tot;
				if (curr > max) {
					max = curr;
				} else {
					curr = 0;
				}
			}
		}
		val["bound"]["bottom"]["tot"] = tot;
		val["bound"]["bottom"]["max"] = max;
	}
	// up
	{
		int tot = 0, max = 0;
		int curr = 0;
		for (int x = 0; x < img.GetWidth(); ++x) {
			if (!IsTransparent(img, x, r.yMax - 1)) {
				++curr;
				++tot;
				if (curr > max) {
					max = curr;
				} else {
					curr = 0;
				}
			}
		}
		val["bound"]["up"]["tot"] = tot;
		val["bound"]["up"]["max"] = max;
	}
}

bool TrimImage::IsTransparent(const d2d::ImageData& img, int x, int y) const
{
	if (img.GetChannels() != 4) {
		return false;
	} else {
		return img.GetPixelData()[(img.GetWidth() * y + x) * 4 + 3] == 0;
	}
}

}