#pragma once

#include <wx/wx.h>
#include <json/json.h>
#include <drag2d.h>
#include <rapidxml-1.13/rapidxml.hpp>

namespace eanim
{
	class Layer;
	class KeyFrame;
	class SkeletonData;

	class FileIO
	{
	public:
		static void load(const wxString& filepath);
		static void store(const wxString& filepath);
		static void reload();

		static void loadFlash(const wxString& filepath);

		static void storeAsGif(const wxString& src, const wxString& dst);

	private:
		static void loadResource(const Json::Value& resValue);
		static void storeResource(Json::Value& value);

		static Layer* loadLayer(const Json::Value& layerValue,
			const wxString& dir);
		static KeyFrame* loadFrame(const Json::Value& frameValue,
			const wxString& dir);
		static d2d::ISprite* loadActor(const Json::Value& actorValue,
			const wxString& dir);
		static void loadSkeleton(const Json::Value& skeletonValue, 
			const std::vector<d2d::ISprite*>& sprites, SkeletonData& skeleton);

		static Layer* loadLayer(rapidxml::xml_node<>* layerNode, 
			const std::map<std::string, std::string>& mapNamePath);
		static KeyFrame* loadFrame(rapidxml::xml_node<>* frameNode,
			const std::map<std::string, std::string>& mapNamePath);
		static d2d::ISprite* loadActor(rapidxml::xml_node<>* actorNode,
			const std::map<std::string, std::string>& mapNamePath);

		static Json::Value store(Layer* layer, const wxString& dir);
		static Json::Value store(KeyFrame* frame, const wxString& dir);
		static Json::Value store(const d2d::ISprite* sprite, const wxString& dir);
		static Json::Value storeSkeleton(const SkeletonData& skeleton);

		static unsigned char* formatRGBA(const unsigned char* rgba, int width, int height);

	private:
		static std::string filepath;

	}; // FileIO
}

