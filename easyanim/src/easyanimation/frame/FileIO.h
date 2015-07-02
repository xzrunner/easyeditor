#ifndef _EASYANIM_FILE_IO_H_
#define _EASYANIM_FILE_IO_H_

#include <wx/wx.h>
#include <json/json.h>
#include <drag2d.h>
#include <rapidxml.hpp>

namespace eanim
{

class Layer;
class KeyFrame;
class SkeletonData;
class Controller;

class FileIO
{
public:
	static void load(const wxString& filepath, Controller* ctrl);
	static void store(const wxString& filepath, Controller* ctrl);
	static void reload(Controller* ctrl);

	static void loadFlash(const wxString& filepath, Controller* ctrl);

	static void storeAsGif(const wxString& src, const wxString& dst);
	static void storeAsPng(const wxString& src, const wxString& dst);

private:
	static Layer* loadLayer(const Json::Value& layerValue,
		const wxString& dir, Controller* ctrl);
	static KeyFrame* loadFrame(const Json::Value& frameValue,
		const wxString& dir, Controller* ctrl);
	static d2d::ISprite* loadActor(const Json::Value& actorValue,
		const wxString& dir, Controller* ctrl);
	static void loadSkeleton(const Json::Value& skeletonValue, 
		const std::vector<d2d::ISprite*>& sprites, SkeletonData& skeleton);

	static Layer* loadLayer(rapidxml::xml_node<>* layerNode, 
		const std::map<std::string, std::string>& mapNamePath, Controller* ctrl);
	static KeyFrame* loadFrame(rapidxml::xml_node<>* frameNode,
		const std::map<std::string, std::string>& mapNamePath, Controller* ctrl);
	static d2d::ISprite* loadActor(rapidxml::xml_node<>* actorNode,
		const std::map<std::string, std::string>& mapNamePath);

	static Json::Value store(Layer* layer, const wxString& dir, Controller* ctrl);
	static Json::Value store(KeyFrame* frame, const wxString& dir, Controller* ctrl);
	static Json::Value store(const d2d::ISprite* sprite, const wxString& dir, Controller* ctrl);
	static Json::Value storeSkeleton(const SkeletonData& skeleton);

private:
	static std::string filepath;

}; // FileIO

}

#endif // _EASYANIM_FILE_IO_H_