#ifndef _EASYANIM_FLASH_LOADER_H_
#define _EASYANIM_FLASH_LOADER_H_

#include <rapidxml.hpp>

#include <string>
#include <map>

namespace ee { class Sprite; }

namespace eanim
{

class Layer;
class KeyFrame;

class FlashLoader
{
public:
	static void Load(const std::string& filepath);

private:
	static Layer* LoadLayer(rapidxml::xml_node<>* layer_node, 
		const std::map<std::string, std::string>& map_name_path);
	static KeyFrame* LoadFrame(Layer* layer, rapidxml::xml_node<>* frame_node,
		const std::map<std::string, std::string>& map_name_path);
	static ee::Sprite* LoadActor(rapidxml::xml_node<>* actor_node,
		const std::map<std::string, std::string>& map_name_path);

}; // FlashLoader

}

#endif // _EASYANIM_FLASH_LOADER_H_