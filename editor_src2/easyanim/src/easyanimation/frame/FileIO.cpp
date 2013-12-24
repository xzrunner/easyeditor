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

	return layer;
}

KeyFrame* FileIO::loadFrame(const Json::Value& frameValue, const wxString& dlg)
{
	int time = frameValue["time"].asInt();

	KeyFrame* frame = new KeyFrame(time);
	std::vector<d2d::ISprite*>& sprites = frame->getAllSprites();

	frame->m_id = frameValue["id"].asInt();

	frame->setClassicTween(frameValue["tween"].asBool());

	int i = 0;
	Json::Value actorValue = frameValue["actor"][i++];
	while (!actorValue.isNull()) {
		d2d::ISprite* actor = loadActor(actorValue, dlg);
		sprites.push_back(actor);
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

	sprite->name = actorValue["name"].asString();

	sprite->multiColor = actorValue["multi color"].asString();
	sprite->addColor = actorValue["add color"].asString();

	d2d::Vector pos;
	pos.x = actorValue["position"]["x"].asDouble();
	pos.y = actorValue["position"]["y"].asDouble();
	float angle = actorValue["angle"].asDouble();
	sprite->setTransform(pos, angle);

	float xscale = actorValue["x scale"].asDouble(),
		yscale = actorValue["y scale"].asDouble();
	sprite->setScale(xscale, yscale);

	if (!actorValue["x shear"].isNull())
	{
		float xshear = actorValue["x shear"].asDouble(),
			yshear = actorValue["y shear"].asDouble(); 
		sprite->setShear(xshear, yshear);
	}

	bool xMirror = actorValue["x mirror"].asBool(), 
		yMirror = actorValue["y mirror"].asBool();
	sprite->setMirror(xMirror, yMirror);

	return sprite;
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
	std::vector<d2d::ISprite*>& sprites = frame->getAllSprites();

	rapidxml::xml_node<>* actorNode = frameNode->first_node("elements")
		->first_node("DOMSymbolInstance");
	while (actorNode) {
		d2d::ISprite* actor = loadActor(actorNode, mapNamePath);
		sprites.push_back(actor);
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
	std::map<int, KeyFrame*>::const_iterator itr = frames.begin();
	for (size_t i = 0; itr != frames.end(); ++itr, ++i)
		value["frame"][i] = store(itr->second, dlg);

	return value;
}

Json::Value FileIO::store(KeyFrame* frame, const wxString& dlg)
{
	Json::Value value;

	value["id"] = frame->m_id;

	value["time"] = frame->getTime();

	value["tween"] = frame->hasClassicTween();

	const std::vector<d2d::ISprite*>& sprites = frame->getAllSprites();
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		value["actor"][i] = store(sprites[i], dlg);

	return value;
}

Json::Value FileIO::store(d2d::ISprite* sprite, const wxString& dlg)
{
	Json::Value value;

// 	wxFileName filename(sprite->getSymbol().getFilepath());
// 	filename.MakeRelativeTo(dlg);
// 	value["filepath"] = filename.GetFullPath().ToStdString();

	if (Context::Instance()->resource.empty())
		value["filepath"] = d2d::FilenameTools::getRelativePath(dlg, 
			sprite->getSymbol().getFilepath()).ToStdString();
	else
		value["filepath"] = d2d::FilenameTools::getFilenameWithExtension(
			sprite->getSymbol().getFilepath()).ToStdString();

	value["name"] = sprite->name;

	value["multi color"] = sprite->multiColor;
	value["add color"] = sprite->addColor;

	value["position"]["x"] = sprite->getPosition().x;
	value["position"]["y"] = sprite->getPosition().y;

	value["angle"] = sprite->getAngle();

	value["x scale"] = sprite->getScaleX();
	value["y scale"] = sprite->getScaleY();

	value["x shear"] = sprite->getShearX();
	value["y shear"] = sprite->getShearY();

	bool xMirror, yMirror;
	sprite->getMirror(xMirror, yMirror);
	value["x mirror"] = xMirror;
	value["y mirror"] = yMirror;

	return value;
}
} // eanim