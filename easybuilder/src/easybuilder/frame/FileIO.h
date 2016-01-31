
#ifndef EBUILDER_FILE_IO_H
#define EBUILDER_FILE_IO_H

#include <wx/wx.h>
#include <json/json.h>



namespace ebuilder
{
	class LibraryPanel;
	class Scene;
	class SceneItem;
	class Layer;
	class LayerItem;
	class Actor;
	class Behavior;

	class FileIO
	{
	public:
		static void load(const wxString& filepath);
		static void store(const wxString& filepath);

	private:
		static void loadGame(const Json::Value& gameValue);
		static SceneItem* loadScene(const Json::Value& sceneValue,
			std::vector<std::pair<Behavior*, wxString> >& buffer);
		static LayerItem* loadLayer(const Json::Value& layerValue,
			std::vector<std::pair<Behavior*, wxString> >& buffer);
		static Actor* loadActor(const Json::Value& actorValue,
			std::vector<std::pair<Behavior*, wxString> >& buffer);
		static ee::TextSprite* loadText(const Json::Value& textValue);
		static Behavior* loadBehavior(const Json::Value& behaviorValue,
			Actor* actor, std::vector<std::pair<Behavior*, wxString> >& buffer);
		static void loadBehaviorsValue(
			const std::vector<std::pair<Behavior*, wxString> >& buffer);

		static void storeGame(Json::Value& value);
		static Json::Value store(Scene* scene, const wxString& filepath);
		static Json::Value store(Layer* layer, const wxString& filepath);
		static Json::Value store(Actor* actor, const wxString& filepath);
		static Json::Value store(ee::TextSprite* text, const wxString& filepath);
		static Json::Value store(Behavior* behavior);
		static void store(Json::Value& value, Behavior* behavior);

	}; // FileIO
}

#endif // EBUILDER_FILE_IO_H
