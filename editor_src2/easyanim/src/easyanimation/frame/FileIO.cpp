#include "FileIO.h"
#include "Context.h"

#include "dataset/Layer.h"
#include "dataset/KeyFrame.h"
#include "view/ToolbarPanel.h"
#include "view/LayersPanel.h"
#include "view/StagePanel.h"

#include <rapidxml-1.13/rapidxml_utils.hpp>

namespace eanim
{

std::string FileIO::filepath;

void FileIO::load(const wxString& filepath)
{
	FileIO::filepath = filepath;

	Context* context = Context::Instance();

	Json::Value value;
	Json::Reader reader;
	std::ifstream fin(filepath.fn_str());
	reader.parse(fin, value);
	fin.close();

	std::string dlg = d2d::FilenameTools::getFileDir(filepath);
	d2d::Context::Instance()->resPath = dlg + "\\";

	context->fps = value["fps"].asInt();
	static_cast<LayersPanel*>(context->layersPanel)->setFPS(context->fps);

	if (!value["resource"].isNull())
		loadResource(value["resource"]);
	else
		context->resource.clear();

	Context::Instance()->name = value["name"].asString();

	int i = 0;
	Json::Value layerValue = value["layer"][i++];
	while (!layerValue.isNull()) {
		Layer* layer = loadLayer(layerValue, dlg);
		Context::Instance()->layers.insertLayer(layer);
		layerValue = value["layer"][i++];
	}

	d2d::LibraryPanel* library = static_cast<d2d::LibraryPanel*>(context->library);
	library->loadFromSymbolMgr(*d2d::SymbolMgr::Instance());
}

void FileIO::store(const wxString& filepath)
{
	Json::Value value;

	storeResource(value);

//	value["name"] = d2d::FilenameTools::getFilename(filepath).ToStdString();
	value["name"] = Context::Instance()->name;

	value["fps"] = Context::Instance()->fps;

	std::string dlg = d2d::FilenameTools::getFileDir(filepath);
	const std::vector<Layer*>& layers = Context::Instance()->layers.getAllLayers();
	for (size_t i = 0, n = layers.size(); i < n; ++i)
		value["layer"][i] = store(layers[i], dlg);

	Json::StyledStreamWriter writer;
	std::ofstream fout(filepath.fn_str());
	writer.write(fout, value);
	fout.close();
}

void FileIO::reload()
{
	if (filepath.empty()) return;

	Context* context = Context::Instance();

	static_cast<d2d::LibraryPanel*>(context->library)->clear();
	static_cast<StagePanel*>(context->stage)->clear();
	context->layers.clear();

	Json::Value value;
	Json::Reader reader;
	std::ifstream fin(filepath.c_str());
	reader.parse(fin, value);
	fin.close();

	std::string dlg = d2d::FilenameTools::getFileDir(filepath);
	int i = 0;
	Json::Value layerValue = value["layer"][i++];
	while (!layerValue.isNull()) {
		Layer* layer = loadLayer(layerValue, dlg);
		Context::Instance()->layers.insertLayer(layer);
		layerValue = value["layer"][i++];
	}

	d2d::LibraryPanel* library = static_cast<d2d::LibraryPanel*>(context->library);
	library->loadFromSymbolMgr(*d2d::SymbolMgr::Instance());

	context->stage->Refresh();
}

void FileIO::loadFlash(const wxString& filepath)
{
	rapidxml::file<> xmlFile(filepath.c_str());
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	
	std::map<std::string, std::string> mapNamePath;
	rapidxml::xml_node<>* imageNode = doc.first_node()->first_node("media")
		->first_node("DOMBitmapItem");
	wxString dlgpath = d2d::FilenameTools::getFileDir(filepath) + "\\";
	while (imageNode) {
		std::string name = imageNode->first_attribute("name")->value();
		name = d2d::FilenameTools::getFilePathExceptExtension(name);

		std::string path = imageNode->first_attribute("sourceExternalFilepath")->value();
		wxString absolutePath = d2d::FilenameTools::getAbsolutePath(dlgpath, path);

		mapNamePath.insert(std::make_pair(name, absolutePath));

		imageNode = imageNode->next_sibling();
	}

	rapidxml::xml_node<>* layerNode = doc.first_node()->first_node("timelines")
		->first_node("DOMTimeline")->first_node("layers")->first_node("DOMLayer");
	while (layerNode) {
		Layer* layer = loadLayer(layerNode, mapNamePath);
		Context::Instance()->layers.insertLayer(layer);
		layerNode = layerNode->next_sibling();
	}

	d2d::LibraryPanel* library = static_cast<d2d::LibraryPanel*>(Context::Instance()->library);
	library->loadFromSymbolMgr(*d2d::SymbolMgr::Instance());
}

void FileIO::loadResource(const Json::Value& resValue)
{
	ResourceMgr& res = Context::Instance()->resource;
	res.clear();

	std::vector<std::string> anims;

	int i = 0;
	Json::Value value = resValue[i++];
	while (!value.isNull()) {
		ResourceMgr::Item item;
		item.path = value["path"].asString();
		item.name = value["name"].asString();
		res.items.push_back(item);
		anims.push_back(item.name);
		value = resValue[i++];
	}

	static_cast<ToolbarPanel*>(Context::Instance()->toolbar)->addAnimChoice(anims);
}

void FileIO::storeResource(Json::Value& value)
{
	ResourceMgr& res = Context::Instance()->resource;
	if (res.empty()) return;

	for (size_t i = 0, n = res.items.size(); i < n; ++i)
	{
		Json::Value itemVal;
		itemVal["path"] = res.items[i].path;
		itemVal["name"] = res.items[i].name;
		value["resource"][i] = itemVal;
	}
}

Layer* FileIO::loadLayer(const Json::Value& layerValue, const wxString& dlg)
{
	Layer* layer = new Layer;

	layer->name = layerValue["name"].asString();

	int i = 0;
	Json::Value frameValue = layerValue["frame"][i++];
	while (!frameValue.isNull()) {
		KeyFrame* frame = loadFrame(frameValue, dlg);
		layer->insertKeyFrame(frame);
		frameValue = layerValue["frame"][i++];
	}

	const std::map<int, KeyFrame*>& frames = layer->getAllFrames();
	std::vector<KeyFrame*> all_frames;
	all_frames.reserve(frames.size());
	std::map<int, KeyFrame*>::const_iterator itr = frames.begin();
	for ( ; itr != frames.end(); ++itr)
		all_frames.push_back(itr->second);
	loadConnection(layerValue, all_frames);

	return layer;
}

KeyFrame* FileIO::loadFrame(const Json::Value& frameValue, const wxString& dlg)
{
	int time = frameValue["time"].asInt();

	KeyFrame* frame = new KeyFrame(time);

	frame->m_id = frameValue["id"].asInt();

	frame->setClassicTween(frameValue["tween"].asBool());

	int i = 0;
	Json::Value actorValue = frameValue["actor"][i++];
	while (!actorValue.isNull()) {
		d2d::ISprite* actor = loadActor(actorValue, dlg);
		frame->insert(actor);
		actorValue = frameValue["actor"][i++];
	}

	return frame;
}

d2d::ISprite* FileIO::loadActor(const Json::Value& actorValue, const wxString& dlg)
{
	std::string filepath = actorValue["filepath"].asString();
	while (true) 
	{
		if (d2d::FilenameTools::isExist(filepath))
			break;

		std::string tmp = d2d::FilenameTools::getAbsolutePath(dlg, filepath);
		if (d2d::FilenameTools::isExist(tmp))
		{
			filepath = tmp;
			break;
		}

		if (!Context::Instance()->resource.empty())
			tmp = Context::Instance()->resource.dir() + "/" + filepath;
		if (!d2d::FilenameTools::isExist(tmp))
			tmp = d2d::FilenameTools::getAbsolutePath(dlg, tmp);
		if (!d2d::FilenameTools::isExist(tmp))
		{
			throw d2d::Exception("File: %s don't exist!", filepath.c_str());
			assert(0);
		}

		filepath = tmp;
		break;
	}

	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->getSymbol(filepath);
	d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
	sprite->load(actorValue);

	return sprite;
}

void FileIO::loadConnection(const Json::Value& val, const std::vector<KeyFrame*>& frames)
{
	bool loaded = false;

	int i = 0;
	Json::Value frameValue = val["frame"][i++];
	while (!frameValue.isNull()) {
		const std::vector<Sprite*>& sprites = frames[i-1]->getAllSprites();
		int j = 0;
		Json::Value actorValue = frameValue["actor"][j++];
		while (!actorValue.isNull()) {
			if (!actorValue["child"].isNull()) {
				int idx = actorValue["child"].asInt();
				const std::vector<Sprite*>& next_sprites = frames[i-1+1]->getAllSprites();
				sprites[j-1]->child = next_sprites[idx];
				loaded = true;
			}
			actorValue = frameValue["actor"][j++];
		}
		frameValue = val["frame"][i++];
	}

	if (!loaded)
	{
		for (int i = 0, n = frames.size() - 1; i < n; ++i)
		{
			const std::vector<Sprite*>& curr_data = frames[i]->getAllSprites();
			const std::vector<Sprite*>& next_data = frames[i+1]->getAllSprites();
			if (curr_data.size() != next_data.size())
				continue;
			for (int j = 0; j < curr_data.size(); ++j)
				for (int k = 0; k < next_data.size(); ++k) {
					Sprite* curr = curr_data[j];
					Sprite* next = next_data[k];
					if (&curr->curr->getSymbol() == &next->curr->getSymbol() &&
						curr->curr->getMirrorX() == next->curr->getMirrorX() &&
						curr->curr->getMirrorY() == next->curr->getMirrorY())
						curr->child = next;
				}
		}
	}
}

Layer* FileIO::loadLayer(rapidxml::xml_node<>* layerNode,
						 const std::map<std::string, std::string>& mapNamePath)
{
	Layer* layer = new Layer;

	layer->name = layerNode->first_attribute("name")->value();

	rapidxml::xml_node<>* frameNode = layerNode->first_node("frames")
		->first_node("DOMFrame");
	while (frameNode) {
		KeyFrame* frame = loadFrame(frameNode, mapNamePath);
		layer->insertKeyFrame(frame);
		frameNode = frameNode->next_sibling();
	}

	return layer;
}

KeyFrame* FileIO::loadFrame(rapidxml::xml_node<>* frameNode,
							const std::map<std::string, std::string>& mapNamePath)
{
	int time = StringTools::stringToInt(frameNode->first_attribute("index")->value()) + 1;

	KeyFrame* frame = new KeyFrame(time);
	rapidxml::xml_node<>* actorNode = frameNode->first_node("elements")
		->first_node("DOMSymbolInstance");
	while (actorNode) {
		d2d::ISprite* actor = loadActor(actorNode, mapNamePath);
		frame->insert(actor);
		actorNode = actorNode->next_sibling();
	}

	return frame;
}

d2d::ISprite* FileIO::loadActor(rapidxml::xml_node<>* actorNode,
								const std::map<std::string, std::string>& mapNamePath)
{
	std::string name = actorNode->first_attribute("libraryItemName")->value();
	std::string filepath = mapNamePath.find(name)->second;
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->getSymbol(filepath);
	d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);

	rapidxml::xml_node<>* matrixNode = actorNode->first_node("matrix")->first_node("Matrix");
	std::string stx = matrixNode->first_attribute("tx")->value();
	std::string sty = matrixNode->first_attribute("ty")->value();
	double tx, ty;
	wxString(stx).ToDouble(&tx);
	wxString(sty).ToDouble(&ty);

	rapidxml::xml_node<>* pointNode = actorNode->first_node("transformationPoint")->first_node("Point");
	std::string sx = pointNode->first_attribute("x")->value();
	std::string sy = pointNode->first_attribute("y")->value();
	double x, y;
	wxString(sx).ToDouble(&x);
	wxString(sy).ToDouble(&y);

	sprite->setTransform(d2d::Vector(float(tx+x), float(ty+y)), 0);

	return sprite;
}

Json::Value FileIO::store(Layer* layer, const wxString& dlg)
{
	Json::Value value;

	value["name"] = layer->name;

	const std::map<int, KeyFrame*>& frames = layer->getAllFrames();
	std::vector<KeyFrame*> all_frames;
	all_frames.reserve(frames.size());
	std::map<int, KeyFrame*>::const_iterator itr = frames.begin();
	for (size_t i = 0; itr != frames.end(); ++itr, ++i) {
		value["frame"][i] = store(itr->second, dlg);
		all_frames.push_back(itr->second);
	}

	storeConnection(value, all_frames);

	return value;
}

Json::Value FileIO::store(KeyFrame* frame, const wxString& dlg)
{
	Json::Value value;

	value["id"] = frame->m_id;

	value["time"] = frame->getTime();

	value["tween"] = frame->hasClassicTween();

	for (size_t i = 0, n = frame->size(); i < n; ++i)
		value["actor"][i] = store(frame->getSprite(i), dlg);

	return value;
}

Json::Value FileIO::store(const d2d::ISprite* sprite, const wxString& dlg)
{
	Json::Value value;

	if (Context::Instance()->resource.empty())
		value["filepath"] = d2d::FilenameTools::getRelativePath(dlg, 
			sprite->getSymbol().getFilepath()).ToStdString();
	else
		value["filepath"] = d2d::FilenameTools::getFilenameWithExtension(
			sprite->getSymbol().getFilepath()).ToStdString();

	sprite->store(value);

	return value;
}

void FileIO::storeConnection(Json::Value& val, const std::vector<KeyFrame*>& frames)
{
	for (int i = 0, n = frames.size() - 1; i < n; ++i)
	{
		KeyFrame* curr = frames[i];
		KeyFrame* next = frames[i+1];
		const std::vector<Sprite*>& curr_data = curr->getAllSprites();
		const std::vector<Sprite*>& next_data = next->getAllSprites();
		for (int j = 0, m = curr_data.size(); j < m; ++j)
		{
			Sprite* parent = curr_data[j];
			if (parent->child)
			{
				for (int k = 0; k < next_data.size(); ++k)
				{
					if (parent->child == next_data[k])
						val["frame"][i]["actor"][j]["child"] = k;
				}
			}
		}
	}
}

} // eanim