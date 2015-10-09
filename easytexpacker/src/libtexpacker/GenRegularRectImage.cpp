#include "GenRegularRectImage.h"

#include <easyimage.h>

namespace libtp
{

void GenRegularRectImage::CreateMulti(const wxString& filepath)
{
	int i = 1;
	while (true) {
		wxString path = filepath + wxString::FromDouble(i) + ".json";
		if (wxFileName::FileExists(path)) {
			CreateSingle(path);
		} else {
			break;
		}
		++i;
	}
}

void GenRegularRectImage::CreateSingle(const wxString& filepath)
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
		std::string filepath = spr_val["filepath"].asString();
		d2d::Image* img = d2d::ImageMgr::Instance()->GetItem(filepath);
		assert(img->GetOriginWidth() == w && img->GetOriginHeight() == h 
			|| img->GetOriginWidth() == h && img->GetOriginHeight() == w);
		bool rot = img->GetOriginWidth() != w && img->GetOriginHeight() != h;
		pack.AddImage(img, x ,y, w, h, rot, true);
		img->Release();

		spr_val = value["parts"][i++];
	}

	wxString out_file = filepath;
	out_file.Replace(".json", ".png");
	pack.OutputToFile(out_file);
}

}