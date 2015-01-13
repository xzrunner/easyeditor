#include "FileIO.h"
#include "Context.h"

#include "dataset/Layer.h"
#include "dataset/KeyFrame.h"
#include "dataset/Joint.h"
#include "view/ToolbarPanel.h"
#include "view/LayersPanel.h"
#include "view/StagePanel.h"

#include <rapidxml-1.13/rapidxml_utils.hpp>
#include <easyanim.h>
#include <easyimage.h>

namespace eanim
{

std::string FileIO::filepath;

void FileIO::load(const wxString& filepath)
{
	FileIO::filepath = filepath;

	Context* context = Context::Instance();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.fn_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = d2d::FilenameTools::getFileDir(filepath);

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
		Layer* layer = loadLayer(layerValue, dir);
		Context::Instance()->layers.insertLayer(layer);
		layerValue = value["layer"][i++];
	}

	d2d::LibraryPanel* library = static_cast<d2d::LibraryPanel*>(context->library);
	library->loadFromSymbolMgr(*d2d::SymbolMgr::Instance());

	context->setCurrFrame(0, 1);

 	d2d::EditPanel* stage = static_cast<d2d::EditPanel*>(context->stage);
 	stage->getCanvas()->resetViewport();
}

void FileIO::store(const wxString& filepath)
{
	Json::Value value;

	storeResource(value);

//	value["name"] = d2d::FilenameTools::getFilename(filepath).ToStdString();
	value["name"] = Context::Instance()->name;

	value["fps"] = Context::Instance()->fps;

	std::string dir = d2d::FilenameTools::getFileDir(filepath);
	const std::vector<Layer*>& layers = Context::Instance()->layers.getAllLayers();
	for (size_t i = 0, n = layers.size(); i < n; ++i)
		value["layer"][i] = store(layers[i], dir);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.fn_str());
	std::locale::global(std::locale("C"));	
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
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = d2d::FilenameTools::getFileDir(filepath);
	int i = 0;
	Json::Value layerValue = value["layer"][i++];
	while (!layerValue.isNull()) {
		Layer* layer = loadLayer(layerValue, dir);
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

void FileIO::storeAsGif(const wxString& src, const wxString& dst)
{
	if (!d2d::FileNameParser::isType(src, d2d::FileNameParser::e_anim)) {
		return;
	}

	d2d::Snapshoot ss;
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(src);
	anim::Symbol* anim = static_cast<anim::Symbol*>(symbol);

	int max_frame = anim->getMaxFrameIndex();
	int width = symbol->getSize().xLength();
	int height = symbol->getSize().yLength();
	AnimatedGifSaver saver(width, height);
	for (int i = 0; i < max_frame; ++i)
	{
		anim->setFrameIndex(i + 1);
		uint8_t* rgba = ss.outputToMemory(symbol, true);

		uint8_t* rgb = eimage::RGBA2RGB(rgba, width, height, true);
		saver.AddFrame(rgb, 1.0f / anim->getFPS());
		delete[] rgba;
		delete[] rgb;
	}
	anim->setFrameIndex(0);
	saver.Save(dst.c_str());

	symbol->Release();
}

void FileIO::storeAsPng(const wxString& src, const wxString& dst)
{
	if (!d2d::FileNameParser::isType(src, d2d::FileNameParser::e_anim)) {
		return;
	}

	d2d::Snapshoot ss;
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(src);
	ss.outputToImageFile(symbol, dst.ToStdString());
	symbol->Release();
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

Layer* FileIO::loadLayer(const Json::Value& layerValue, const wxString& dir)
{
	Layer* layer = new Layer;

	layer->name = layerValue["name"].asString();

	int i = 0;
	Json::Value frameValue = layerValue["frame"][i++];
	while (!frameValue.isNull()) {
		KeyFrame* frame = loadFrame(frameValue, dir);
		layer->insertKeyFrame(frame);
		frameValue = layerValue["frame"][i++];
	}

	const std::map<int, KeyFrame*>& frames = layer->getAllFrames();
	std::vector<KeyFrame*> all_frames;
	all_frames.reserve(frames.size());
	std::map<int, KeyFrame*>::const_iterator itr = frames.begin();
	for ( ; itr != frames.end(); ++itr)
		all_frames.push_back(itr->second);

	return layer;
}

KeyFrame* FileIO::loadFrame(const Json::Value& frameValue, const wxString& dir)
{
	int time = frameValue["time"].asInt();

	KeyFrame* frame = new KeyFrame(time);

	frame->m_id = frameValue["id"].asInt();

	frame->setClassicTween(frameValue["tween"].asBool());

	int i = 0;
	Json::Value actorValue = frameValue["actor"][i++];
	while (!actorValue.isNull()) {
		d2d::ISprite* actor = loadActor(actorValue, dir);
		frame->insertWithClone(actor);
		actor->Release();
		actorValue = frameValue["actor"][i++];
	}

	loadSkeleton(frameValue["skeleton"], frame->getAllSprites(), frame->getSkeletonData());

	return frame;
}

d2d::ISprite* FileIO::loadActor(const Json::Value& actorValue, const wxString& dir)
{
	std::string filepath = actorValue["filepath"].asString();
	while (true) 
	{
		if (d2d::FilenameTools::isExist(filepath))
			break;

		std::string tmp = d2d::FilenameTools::getAbsolutePath(dir, filepath);
		if (d2d::FilenameTools::isExist(tmp))
		{
			filepath = tmp;
			break;
		}

		if (!Context::Instance()->resource.empty())
			tmp = Context::Instance()->resource.dir() + "/" + filepath;
		if (!d2d::FilenameTools::isExist(tmp))
			tmp = d2d::FilenameTools::getAbsolutePath(dir, tmp);
		if (!d2d::FilenameTools::isExist(tmp))
		{
			throw d2d::Exception("File: %s don't exist!", filepath.c_str());
			assert(0);
		}

		filepath = tmp;
		break;
	}

	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
//	symbol->refresh();
	d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
	sprite->load(actorValue);
	symbol->Release();

	return sprite;
}

void FileIO::loadSkeleton(const Json::Value& skeletonValue, const std::vector<d2d::ISprite*>& sprites,
						  SkeletonData& skeleton)
{
	// prepare joints
	std::map<int, Joint*> mapJoint;
	
	int i = 0;
	Json::Value jointsVal = skeletonValue[i++];
	while (!jointsVal.isNull()) {
		std::string spriteName = jointsVal["sprite"].asString();
		d2d::ISprite* sprite = NULL;
		for (int i = 0, n = sprites.size(); i < n; ++i)
		{
			if (sprites[i]->name == spriteName) 
			{
				sprite = sprites[i];
				break;
			}
		}
		assert(sprite);
		
		int j = 0;
		Json::Value jointVal = jointsVal["joints"][j++];
		while (!jointVal.isNull()) {
			Joint* joint = new Joint(sprite);			
			joint->m_id = jointVal["id"].asInt();
			joint->m_relative.x = jointVal["rx"].asDouble();
			joint->m_relative.y = jointVal["ry"].asDouble();
			mapJoint.insert(std::make_pair(joint->m_id, joint));

			jointVal = jointsVal["joints"][j++];
		}
		jointsVal = skeletonValue[i++];
	}

	// skeleton
	i = 0;
	jointsVal = skeletonValue[i++];
	while (!jointsVal.isNull()) {
		std::string spriteName = jointsVal["sprite"].asString();
		d2d::ISprite* sprite = NULL;
		for (int i = 0, n = sprites.size(); i < n; ++i)
		{
			if (sprites[i]->name == spriteName) 
			{
				sprite = sprites[i];
				break;
			}
		}
		assert(sprite);

		std::vector<Joint*> joints;

		int j = 0;
		Json::Value jointVal = jointsVal["joints"][j++];
		while (!jointVal.isNull()) {
			int joint_id = jointVal["id"].asInt();
			std::map<int, Joint*>::iterator itr = mapJoint.find(joint_id);
			assert(itr != mapJoint.end());
			Joint* joint = itr->second;
			if (!jointVal["parent"].isNull()) {
				itr = mapJoint.find(jointVal["parent"].asInt());
				assert(itr != mapJoint.end());
				joint->m_parent = itr->second;
			}
			if (!jointVal["children"].isNull()) {
				int k = 0;
				Json::Value childVal = jointVal["children"][k++];
				while (!childVal.isNull()) {
					itr = mapJoint.find(childVal.asInt());
					assert(itr != mapJoint.end());
					joint->m_children.insert(itr->second);
					childVal = jointVal["children"][k++];
				}
			}
			joints.push_back(joint);
			jointVal = jointsVal["joints"][j++];
		}
		skeleton.m_mapJoints.insert(std::make_pair(sprite, joints));
		jointsVal = skeletonValue[i++];
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
		frame->insertWithClone(actor);
		actorNode = actorNode->next_sibling();
	}

	return frame;
}

d2d::ISprite* FileIO::loadActor(rapidxml::xml_node<>* actorNode,
								const std::map<std::string, std::string>& mapNamePath)
{
	std::string name = actorNode->first_attribute("libraryItemName")->value();
	std::string filepath = mapNamePath.find(name)->second;
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
//	symbol->refresh();
	d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
	symbol->Release();

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

Json::Value FileIO::store(Layer* layer, const wxString& dir)
{
	Json::Value value;

	value["name"] = layer->name;

	const std::map<int, KeyFrame*>& frames = layer->getAllFrames();
	std::vector<KeyFrame*> all_frames;
	all_frames.reserve(frames.size());
	std::map<int, KeyFrame*>::const_iterator itr = frames.begin();
	for (size_t i = 0; itr != frames.end(); ++itr, ++i) {
		value["frame"][i] = store(itr->second, dir);
		all_frames.push_back(itr->second);
	}

	return value;
}

Json::Value FileIO::store(KeyFrame* frame, const wxString& dir)
{
	Json::Value value;

	value["id"] = frame->m_id;

	value["time"] = frame->getTime();

	value["tween"] = frame->hasClassicTween();

	for (size_t i = 0, n = frame->size(); i < n; ++i)
		value["actor"][i] = store(frame->getSprite(i), dir);

	value["skeleton"] = storeSkeleton(frame->getSkeletonData());

	return value;
}

Json::Value FileIO::store(const d2d::ISprite* sprite, const wxString& dir)
{
	Json::Value value;

	if (Context::Instance()->resource.empty())
		value["filepath"] = d2d::FilenameTools::getRelativePath(dir, 
			sprite->getSymbol().getFilepath()).ToStdString();
	else
		value["filepath"] = d2d::FilenameTools::getFilenameWithExtension(
			sprite->getSymbol().getFilepath()).ToStdString();

	sprite->store(value);

	return value;
}

Json::Value FileIO::storeSkeleton(const SkeletonData& skeleton)
{
	Json::Value value;

	std::map<d2d::ISprite*, std::vector<Joint*> >::const_iterator itr
		= skeleton.m_mapJoints.begin();
	for (int i = 0; itr != skeleton.m_mapJoints.end(); ++itr, ++i)
	{
		value[i]["sprite"] = itr->first->name;
		for (int j = 0, m = itr->second.size(); j < m; ++j)
		{
			Joint* joint = itr->second[j];

			Json::Value jointVal;
			jointVal["id"] = joint->m_id;
			jointVal["rx"] = joint->m_relative.x;
			jointVal["ry"] = joint->m_relative.y;
			if (joint->m_parent)
				jointVal["parent"] = joint->m_parent->m_id;
			std::set<Joint*>::iterator itr_child = joint->m_children.begin();
			for (int k = 0; itr_child != joint->m_children.end(); ++itr_child, ++k)
				jointVal["children"][k] = (*itr_child)->m_id;

			value[i]["joints"][j] = jointVal;
		}
	}

	return value;
}

} // eanim