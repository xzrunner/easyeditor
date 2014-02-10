#pragma once

#include <wx/wx.h>
#include <json/json.h>
#include <drag2d.h>
#include <rapidxml-1.13/rapidxml.hpp>

namespace eanim
{
	class Layer;
	class KeyFrame;

	class FileIO
	{
	public:
		static void load(const wxString& filepath);
		static void store(const wxString& filepath);
		static void reload();

		static void loadFlash(const wxString& filepath);

	private:
		static void loadResource(const Json::Value& resValue);
		static void storeResource(Json::Value& value);

		static Layer* loadLayer(const Json::Value& layerValue,
			const wxString& dlg);
		static KeyFrame* loadFrame(const Json::Value& frameValue,
			const wxString& dlg);
		static d2d::ISprite* loadActor(const Json::Value& actorValue,
			const wxString& dlg);

		static Layer* loadLayer(rapidxml::xml_node<>* layerNode, 
			const std::map<std::string, std::string>& mapNamePath);
		static KeyFrame* loadFrame(rapidxml::xml_node<>* frameNode,
			const std::map<std::string, std::string>& mapNamePath);
		static d2d::ISprite* loadActor(rapidxml::xml_node<>* actorNode,
			const std::map<std::string, std::string>& mapNamePath);

		static Json::Value store(Layer* layer, const wxString& dlg);
		static Json::Value store(KeyFrame* frame, const wxString& dlg);
		static Json::Value store(const d2d::ISprite* sprite, const wxString& dlg);

	private:
		static std::string filepath;

	}; // FileIO
}

