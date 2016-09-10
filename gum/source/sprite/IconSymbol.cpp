//#include "IconSymbol.h"
//
//namespace gum
//{
//
//void IconSymbol::LoadJson(const std::string& filepath)
//{
//	Json::Value value;
//	Json::Reader reader;
//	std::locale::global(std::locale(""));
//	std::ifstream fin(filepath.c_str());
//	std::locale::global(std::locale("C"));
//	reader.parse(fin, value);
//	fin.close();
//
//	std::string dir = FilepathHelper::Dir(filepath);
//	std::string path = ee::FileHelper::GetAbsolutePath(dir, value["image"].asString());
//	ee::Image* img = ee::ImageMgr::Instance()->GetItem(path);
//
//	Icon* icon = IconFactory::CreateIconFromFile(value);
//	icon->SetImage(img);
//	icon->LoadFromFile(value);
//	img->RemoveReference();
//
//	return icon;
//}
//
//}