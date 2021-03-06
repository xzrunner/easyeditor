#ifndef _EASYANIM_FILE_SAVER_H_
#define _EASYANIM_FILE_SAVER_H_

#include <ee/Sprite.h>

#include <json/json.h>

#include <string>

namespace eanim
{

class Layer;
class KeyFrame;
class SkeletonData;

class FileSaver
{
public:
	static void StoreSingle(const std::string& filepath);
	static void StoreTemplate(const std::string& filepath);

	static void StoreAsGif(const std::string& src, const std::string& dst);
	static void StoreAsPng(const std::string& src, const std::string& dst);

private:
	static Json::Value StoreLayer(Layer* layer, const std::string& dir, bool single);
	static Json::Value StoreFrame(KeyFrame* frame, const std::string& dir, bool single);
	static Json::Value StoreActor(const ee::SprConstPtr& spr, const std::string& dir, bool single);
	static Json::Value StoreSkeleton(const SkeletonData& skeleton);

}; // FileSaver

}

#endif // _EASYANIM_FILE_SAVER_H_