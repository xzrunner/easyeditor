#include "GenRegularRectImage.h"

#include <easyimage.h>

namespace libpacker
{

void GenRegularRectImage::Create(const wxString& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.mb_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	int width = value["width"].asInt(),
		height = value["height"].asInt();
	eimage::ImagePack pack(width, height);

	int i = 0;
	Json::Value spr_val = value["parts"][i++];
	while (!spr_val.isNull()) {
		int x = spr_val["dst"]["x"].asInt(),
			y = spr_val["dst"]["y"].asInt();
		int w = spr_val["dst"]["w"].asInt(),
			h = spr_val["dst"]["h"].asInt();
		wxString filepath = spr_val["filepath"].asString();
		d2d::Image* img = d2d::ImageMgr::Instance()->getItem(filepath);
		pack.AddImage(img, x ,y, w, h);
		img->Release();

		spr_val = value["parts"][i++];
	}

	wxString dir = d2d::FilenameTools::getFileDir(filepath);
	pack.OutputToFile(dir + "\\pack.png");
}

}