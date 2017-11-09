#include "FlashLoader.h"
#include "dataset/Layer.h"
#include "dataset/KeyFrame.h"
#include "message/messages.h"
#include "view/ViewMgr.h"
#include "view/LibraryPanel.h"

#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/StringHelper.h>
#include <ee/Sprite.h>
#include <ee/SpriteFactory.h>

#include <rapidxml_utils.hpp>

namespace eanim
{

void FlashLoader::Load(const std::string& filepath)
{
	rapidxml::file<> xml_file(filepath.c_str());
	rapidxml::xml_document<> doc;
	doc.parse<0>(xml_file.data());

	std::map<std::string, std::string> map_name_path;
	rapidxml::xml_node<>* img_node = doc.first_node()->first_node("media")
		->first_node("DOMBitmapItem");
	std::string dlgpath = ee::FileHelper::GetFileDir(filepath) + "\\";
	while (img_node) 
	{
		std::string name = img_node->first_attribute("name")->value();
		name = ee::FileHelper::GetFilePathExceptExtension(name);

		std::string path = img_node->first_attribute("sourceExternalFilepath")->value();
		std::string absolutePath = ee::FileHelper::GetAbsolutePath(dlgpath, path);

		map_name_path.insert(std::make_pair(name, absolutePath));

		img_node = img_node->next_sibling();
	}

	rapidxml::xml_node<>* layer_node = doc.first_node()->first_node("timelines")
		->first_node("DOMTimeline")->first_node("layers")->first_node("DOMLayer");
	while (layer_node) {
		Layer* layer = LoadLayer(layer_node, map_name_path);
		InsertLayerSJ::Instance()->Insert(layer);
		SetSelectedSJ::Instance()->Set(0, 0);
		layer_node = layer_node->next_sibling();
	}

	ViewMgr::Instance()->library->LoadFromSymbolMgr(*ee::SymbolMgr::Instance());
}

Layer* FlashLoader::LoadLayer(rapidxml::xml_node<>* layer_node,
							  const std::map<std::string, std::string>& map_name_path)
{
	Layer* layer = new Layer;

	layer->SetName(layer_node->first_attribute("name")->value());

	rapidxml::xml_node<>* frameNode = layer_node->first_node("frames")
		->first_node("DOMFrame");
	while (frameNode) {
		KeyFrame* frame = LoadFrame(layer, frameNode, map_name_path);
		layer->InsertKeyFrame(frame);
		frame->RemoveReference();
		frameNode = frameNode->next_sibling();
	}

	return layer;
}

KeyFrame* FlashLoader::LoadFrame(Layer* layer, rapidxml::xml_node<>* frame_node,
								 const std::map<std::string, std::string>& map_name_path)
{
	int time = ee::StringHelper::FromString<int>(frame_node->first_attribute("index")->value()) + 1;

	KeyFrame* frame = new KeyFrame(time);
	frame->SetLayer(layer);

	rapidxml::xml_node<>* actor_node = frame_node->first_node("elements")
		->first_node("DOMSymbolInstance");
	while (actor_node) {
		ee::SprPtr actor = LoadActor(actor_node, map_name_path);
		frame->Insert(actor, INT_MAX);
		actor_node = actor_node->next_sibling();
	}

	return frame;
}

ee::SprPtr FlashLoader::LoadActor(rapidxml::xml_node<>* actor_node,
								   const std::map<std::string, std::string>& map_name_path)
{
	std::string name = actor_node->first_attribute("libraryItemName")->value();
	std::string filepath = map_name_path.find(name)->second;
	auto sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	//	sym->refresh();
	auto spr = ee::SpriteFactory::Instance()->Create(sym);

	rapidxml::xml_node<>* mat_node = actor_node->first_node("matrix")->first_node("Matrix");
	std::string stx = mat_node->first_attribute("tx")->value();
	std::string sty = mat_node->first_attribute("ty")->value();
	double tx, ty;
	ee::StringHelper::FromString(stx, tx);
	ee::StringHelper::FromString(sty, ty);

	rapidxml::xml_node<>* point_node = actor_node->first_node("transformationPoint")->first_node("Point");
	std::string sx = point_node->first_attribute("x")->value();
	std::string sy = point_node->first_attribute("y")->value();
	double x, y;
	ee::StringHelper::FromString(sx, x);
	ee::StringHelper::FromString(sy, y);

	spr->SetPosition(sm::vec2(float(tx+x), float(ty+y)));

	return spr;
}

}