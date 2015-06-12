#include "FileIO.h"

#include "frame/Controller.h"
#include "dataset/Layer.h"
#include "dataset/KeyFrame.h"
#include "dataset/Joint.h"
#include "view/ToolbarPanel.h"
#include "view/LayersPanel.h"
#include "view/StagePanel.h"

#include <rapidxml_utils.hpp>
#include <easyanim.h>
#include <easyimage.h>

namespace eanim
{

std::string FileIO::filepath;

void FileIO::load(const wxString& filepath, Controller* ctrl)
{
	FileIO::filepath = filepath;

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.fn_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = d2d::FilenameTools::getFileDir(filepath);

	ctrl->fps = value["fps"].asInt();
	ctrl->GetLayersPanel()->setFPS(ctrl->fps);

	if (!value["resource"].isNull())
		loadResource(value["resource"], ctrl);
	else
		ctrl->GetResource().clear();

	ctrl->name = value["name"].asString();

	int i = 0;
	Json::Value layerValue = value["layer"][i++];
	while (!layerValue.isNull()) {
		Layer* layer = loadLayer(layerValue, dir, ctrl);
//		ctrl->InsertLayer(layer);
		layerValue = value["layer"][i++];
	}

	ctrl->GetLibraryPanel()->LoadFromSymbolMgr(*d2d::SymbolMgr::Instance());

	ctrl->setCurrFrame(0, 1);

 	ctrl->GetStagePanel()->getCanvas()->resetViewport();
}

void FileIO::store(const wxString& filepath, Controller* ctrl)
{
	Json::Value value;

	storeResource(value, ctrl);

//	value["name"] = d2d::FilenameTools::getFilename(filepath).ToStdString();
	value["name"] = ctrl->name;

	value["fps"] = ctrl->fps;

	std::string dir = d2d::FilenameTools::getFileDir(filepath);
	const std::vector<Layer*>& layers = ctrl->GetLayers().getAllLayers();
	for (size_t i = 0, n = layers.size(); i < n; ++i)
		value["layer"][i] = store(layers[i], dir, ctrl);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.fn_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileIO::reload(Controller* ctrl)
{
	if (filepath.empty()) return;

	ctrl->Clear();

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
		Layer* layer = loadLayer(layerValue, dir, ctrl);
		ctrl->GetLayers().insertLayer(layer);
		layerValue = value["layer"][i++];
	}

	ctrl->GetLibraryPanel()->LoadFromSymbolMgr(*d2d::SymbolMgr::Instance());

	ctrl->Refresh();
}

void FileIO::loadFlash(const wxString& filepath, Controller* ctrl)
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
		Layer* layer = loadLayer(layerNode, mapNamePath, ctrl);
		ctrl->InsertLayer(layer);
		layerNode = layerNode->next_sibling();
	}

	ctrl->GetLibraryPanel()->LoadFromSymbolMgr(*d2d::SymbolMgr::Instance());
}

void FileIO::storeAsGif(const wxString& src, const wxString& dst)
{
	if (!d2d::FileNameParser::isType(src, d2d::FileNameParser::e_anim)) {
		return;
	}

	d2d::Snapshoot ss;
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(src);
	libanim::Symbol* anim = static_cast<libanim::Symbol*>(symbol);

	int max_frame = anim->getMaxFrameIndex();
	int width = symbol->getSize().xLength();
	int height = symbol->getSize().yLength();
	AnimatedGifSaver saver(width, height);
	for (int i = 0; i < max_frame; ++i)
	{
		anim->setFrameIndex(i + 1);
		uint8_t* rgba = ss.OutputToMemory(symbol, true);

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
	ss.OutputToImageFile(symbol, dst.ToStdString());
	symbol->Release();
}

void FileIO::loadResource(const Json::Value& resValue, Controller* ctrl)
{
	ctrl->GetResource().clear();

	std::vector<std::string> anims;

	int i = 0;
	Json::Value value = resValue[i++];
	while (!value.isNull()) {
		ResourceMgr::Item item;
		item.path = value["path"].asString();
		item.name = value["name"].asString();
		ctrl->GetResource().items.push_back(item);
		anims.push_back(item.name);
		value = resValue[i++];
	}

	ctrl->GetToolbarPanel()->addAnimChoice(anims);
}

void FileIO::storeResource(Json::Value& value, Controller* ctrl)
{
	ResourceMgr& res = ctrl->GetResource();
	if (res.empty()) return;

	for (size_t i = 0, n = res.items.size(); i < n; ++i)
	{
		Json::Value itemVal;
		itemVal["path"] = res.items[i].path;
		itemVal["name"] = res.items[i].name;
		value["resource"][i] = itemVal;
	}
}

Layer* FileIO::loadLayer(const Json::Value& layerValue, const wxString& dir, Controller* ctrl)
{
	Layer* layer = new Layer(ctrl);

	ctrl->InsertLayer(layer);

	layer->SetName(layerValue["name"].asString());

	int i = 0;
	Json::Value frameValue = layerValue["frame"][i++];
	while (!frameValue.isNull()) {
		KeyFrame* frame = loadFrame(frameValue, dir, ctrl);
		layer->InsertKeyFrame(frame);
		frame->Release();
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

KeyFrame* FileIO::loadFrame(const Json::Value& frameValue, const wxString& dir, Controller* ctrl)
{
	int time = frameValue["time"].asInt();

	KeyFrame* frame = new KeyFrame(ctrl, time);

	frame->SetID(frameValue["id"].asInt());

	frame->SetClassicTween(frameValue["tween"].asBool());

	int i = 0;
	Json::Value actorValue = frameValue["actor"][i++];
	while (!actorValue.isNull()) {
		d2d::ISprite* actor = loadActor(actorValue, dir, ctrl);
		frame->Insert(actor);
		actor->Release();
		actorValue = frameValue["actor"][i++];
	}

	loadSkeleton(frameValue["skeleton"], frame->GetAllSprites(), frame->GetSkeletonData());

	return frame;
}

d2d::ISprite* FileIO::loadActor(const Json::Value& actorValue, const wxString& dir,
								Controller* ctrl)
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

		if (!ctrl->GetResource().empty())
			tmp = ctrl->GetResource().dir() + "/" + filepath;
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
	d2d::SymbolSearcher::SetSymbolFilepaths(dir, symbol, actorValue);
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
						 const std::map<std::string, std::string>& mapNamePath,
						 Controller* ctrl)
{
	Layer* layer = new Layer(ctrl);

	layer->SetName(layerNode->first_attribute("name")->value());

	rapidxml::xml_node<>* frameNode = layerNode->first_node("frames")
		->first_node("DOMFrame");
	while (frameNode) {
		KeyFrame* frame = loadFrame(frameNode, mapNamePath, ctrl);
		layer->InsertKeyFrame(frame);
		frame->Release();
		frameNode = frameNode->next_sibling();
	}

	return layer;
}

KeyFrame* FileIO::loadFrame(rapidxml::xml_node<>* frameNode,
							const std::map<std::string, std::string>& mapNamePath,
							Controller* ctrl)
{
	int time = StringTools::stringToInt(frameNode->first_attribute("index")->value()) + 1;

	KeyFrame* frame = new KeyFrame(ctrl, time);
	rapidxml::xml_node<>* actorNode = frameNode->first_node("elements")
		->first_node("DOMSymbolInstance");
	while (actorNode) {
		d2d::ISprite* actor = loadActor(actorNode, mapNamePath);
		frame->Insert(actor);
		actor->Release();
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

Json::Value FileIO::store(Layer* layer, const wxString& dir, Controller* ctrl)
{
	Json::Value value;

	value["name"] = layer->GetName();

	const std::map<int, KeyFrame*>& frames = layer->getAllFrames();
	std::vector<KeyFrame*> all_frames;
	all_frames.reserve(frames.size());
	std::map<int, KeyFrame*>::const_iterator itr = frames.begin();
	for (size_t i = 0; itr != frames.end(); ++itr, ++i) {
		value["frame"][i] = store(itr->second, dir, ctrl);
		all_frames.push_back(itr->second);
	}

	return value;
}

Json::Value FileIO::store(KeyFrame* frame, const wxString& dir, Controller* ctrl)
{
	Json::Value value;

	value["id"] = frame->GetID();

	value["time"] = frame->GetTime();

	value["tween"] = frame->HasClassicTween();

	for (size_t i = 0, n = frame->Size(); i < n; ++i)
		value["actor"][i] = store(frame->GetSprite(i), dir, ctrl);

	value["skeleton"] = storeSkeleton(frame->GetSkeletonData());

	return value;
}

Json::Value FileIO::store(const d2d::ISprite* sprite, const wxString& dir,
						  Controller* ctrl)
{
	Json::Value value;

	const d2d::ISymbol& symbol = sprite->getSymbol();
	// filepath
	if (ctrl->GetResource().empty())
		value["filepath"] = d2d::FilenameTools::getRelativePath(dir, 
			symbol.getFilepath()).ToStdString();
	else
		value["filepath"] = d2d::FilenameTools::getFilenameWithExtension(
			symbol.getFilepath()).ToStdString();
	// filepaths
	const std::set<std::string>& filepaths = symbol.GetFilepaths();
	std::set<std::string>::const_iterator itr = filepaths.begin();
	for (int i = 0; itr != filepaths.end(); ++itr, ++i) {
		value["filepaths"][i] = *itr;
	}
	// other
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