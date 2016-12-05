#ifndef _EASYANIM_FILE_IO_H_
#define _EASYANIM_FILE_IO_H_

#include <wx/wx.h>
#include <json/json.h>

#include <rapidxml.hpp>

namespace ee { class Sprite; }

namespace eanim
{

class Layer;
class KeyFrame;
class SkeletonData;

class FileIO
{
public:
	static void Load(const std::string& filepath);
	static void StoreSingle(const std::string& filepath);
	static void StoreTemplate(const std::string& filepath);
	static void Reload();

	static void LoadFlash(const std::string& filepath);

	static void StoreAsGif(const std::string& src, const std::string& dst);
	static void StoreAsPng(const std::string& src, const std::string& dst);

private:
	static Layer* LoadLayer(const Json::Value& layerValue,
		const std::string& dir);
	static KeyFrame* LoadFrame(Layer* layer, const Json::Value& frameValue,
		const std::string& dir);
	static void LoadLerp(KeyFrame* frame, const Json::Value& val);
	static ee::Sprite* LoadActor(const Json::Value& actorValue,
		const std::string& dir);
	static void LoadSkeleton(const Json::Value& skeletonValue, 
		const std::vector<ee::Sprite*>& sprs, SkeletonData& skeleton);

	static Layer* LoadLayer(rapidxml::xml_node<>* layerNode, 
		const std::map<std::string, std::string>& mapNamePath);
	static KeyFrame* LoadFrame(Layer* layer, rapidxml::xml_node<>* frameNode,
		const std::map<std::string, std::string>& mapNamePath);
	static ee::Sprite* LoadActor(rapidxml::xml_node<>* actorNode,
		const std::map<std::string, std::string>& mapNamePath);

	static Json::Value StoreLayer(Layer* layer, const std::string& dir, bool single);
	static Json::Value StoreFrame(KeyFrame* frame, const std::string& dir, bool single);
	static Json::Value StoreLerp(KeyFrame* frame);	
	static Json::Value StoreActor(const ee::Sprite* spr, const std::string& dir, bool single);
	static Json::Value StoreSkeleton(const SkeletonData& skeleton);

private:
	static std::string m_filepath;

}; // FileIO

}

#endif // _EASYANIM_FILE_IO_H_