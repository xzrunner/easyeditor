#ifndef _EASYANIM_DEFAULT_FILE_LOADER_H_
#define _EASYANIM_DEFAULT_FILE_LOADER_H_

#include <ee/Sprite.h>

#include <json/json.h>

namespace eanim
{

class Layer;
class KeyFrame;
class SkeletonData;

class DefaultFileLoader
{
public:
	static void Load(const Json::Value& val, const std::string& dir);

	static void Reload(const Json::Value& val, const std::string& dir);

private:
	static Layer* LoadLayer(const Json::Value& layer_val,
		const std::string& dir);
	static KeyFrame* LoadFrame(Layer* layer, const Json::Value& frame_val,
		const std::string& dir);
	static void LoadLerp(KeyFrame* frame, const Json::Value& val);
	static ee::SprPtr LoadActor(const Json::Value& actor_val,
		const std::string& dir);
	static void LoadSkeleton(const Json::Value& skeleton_val, 
		const std::vector<ee::SprPtr>& sprs, SkeletonData& skeleton);

}; // DefaultFileLoader

}

#endif // _EASYANIM_DEFAULT_FILE_LOADER_H_