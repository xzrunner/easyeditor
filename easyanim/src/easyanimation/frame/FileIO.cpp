#include "FileIO.h"

#include "dataset/DataMgr.h"
#include "dataset/Layer.h"
#include "dataset/KeyFrame.h"
#include "dataset/Joint.h"
#include "view/ViewMgr.h"
#include "view/ToolbarPanel.h"
#include "view/StagePanel.h"
#include "message/messages.h"

#include <rapidxml_utils.hpp>
#include <easyanim.h>
#include <easyimage.h>

namespace eanim
{

std::string FileIO::m_filepath;

void FileIO::Load(const std::string& filepath)
{
	FileIO::m_filepath = filepath;

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = d2d::FileHelper::GetFileDir(filepath);

	int fps = value["fps"].asInt();
	SetFpsSJ::Instance()->Set(fps);

	if (value["template"].isNull()) {
		ViewMgr::Instance()->toolbar->ChangeTemplateMode(true);
	} else {
		DataMgr::Instance()->GetTemplate().LoadFromFile(value["template"]);
		ViewMgr::Instance()->toolbar->ChangeTemplateMode(false);
	}

	DataMgr::Instance()->name = value["name"].asString();

	DataMgr::Instance()->GetLayers().Clear();
	ViewMgr::Instance()->library->Clear();
	SetSelectedSJ::Instance()->Set(-1, -1);

	int i = 0;
	Json::Value layerValue = value["layer"][i++];
	while (!layerValue.isNull()) {
		LoadLayer(layerValue, dir);
		layerValue = value["layer"][i++];
	}

	ViewMgr::Instance()->library->LoadFromSymbolMgr(*d2d::SymbolMgr::Instance());

	SetSelectedSJ::Instance()->Set(0, 0);
}

void FileIO::StoreSingle(const std::string& filepath)
{
	Json::Value value;

	value["name"] = DataMgr::Instance()->name;
	value["fps"] = GetFpsSJ::Instance()->Get();

	std::string dir = d2d::FileHelper::GetFileDir(filepath);

	LayersMgr& layers = DataMgr::Instance()->GetLayers();
	for (size_t i = 0, n = layers.Size(); i < n; ++i) {
		value["layer"][i] = StoreLayer(layers.GetLayer(i), dir, true);
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileIO::StoreTemplate(const std::string& filepath)
{
	Json::Value value;

	AnimTemplate& temp = DataMgr::Instance()->GetTemplate();
	temp.PreparePaths(filepath);
	temp.StoreToFile(value["template"]);

	value["name"] = DataMgr::Instance()->name;
	value["fps"] = GetFpsSJ::Instance()->Get();

	std::string dir = d2d::FileHelper::GetFileDir(filepath);
	LayersMgr& layers = DataMgr::Instance()->GetLayers();
	for (size_t i = 0, n = layers.Size(); i < n; ++i) {
		value["layer"][i] = StoreLayer(layers.GetLayer(i), dir, false);
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileIO::Reload()
{
	if (m_filepath.empty()) return;

	DataMgr::Instance()->GetLayers().Clear();
	ViewMgr::Instance()->library->Clear();
	SetSelectedSJ::Instance()->Set(-1, -1);

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = d2d::FileHelper::GetFileDir(m_filepath);
	int i = 0;
	Json::Value layerValue = value["layer"][i++];
	while (!layerValue.isNull()) {
		Layer* layer = LoadLayer(layerValue, dir);
		layerValue = value["layer"][i++];
	}

	ViewMgr::Instance()->library->LoadFromSymbolMgr(*d2d::SymbolMgr::Instance());
}

void FileIO::LoadFlash(const std::string& filepath)
{
	rapidxml::file<> xmlFile(filepath.c_str());
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	
	std::map<std::string, std::string> mapNamePath;
	rapidxml::xml_node<>* imageNode = doc.first_node()->first_node("media")
		->first_node("DOMBitmapItem");
	std::string dlgpath = d2d::FileHelper::GetFileDir(filepath) + "\\";
	while (imageNode) {
		std::string name = imageNode->first_attribute("name")->value();
		name = d2d::FileHelper::GetFilePathExceptExtension(name);

		std::string path = imageNode->first_attribute("sourceExternalFilepath")->value();
		std::string absolutePath = d2d::FileHelper::GetAbsolutePath(dlgpath, path);

		mapNamePath.insert(std::make_pair(name, absolutePath));

		imageNode = imageNode->next_sibling();
	}

	rapidxml::xml_node<>* layerNode = doc.first_node()->first_node("timelines")
		->first_node("DOMTimeline")->first_node("layers")->first_node("DOMLayer");
	while (layerNode) {
		Layer* layer = LoadLayer(layerNode, mapNamePath);
		InsertLayerSJ::Instance()->Insert(layer);
		SetSelectedSJ::Instance()->Set(0, 0);
		layerNode = layerNode->next_sibling();
	}

	ViewMgr::Instance()->library->LoadFromSymbolMgr(*d2d::SymbolMgr::Instance());
}

void FileIO::StoreAsGif(const std::string& src, const std::string& dst)
{
	if (!d2d::FileType::IsType(src, d2d::FileType::e_anim)) {
		return;
	}

	d2d::Snapshoot ss;
	d2d::Symbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(src);
	libanim::Symbol* anim = static_cast<libanim::Symbol*>(symbol);

	int max_frame = anim->getMaxFrameIndex();
	int width = symbol->GetSize().Width();
	int height = symbol->GetSize().Height();
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

void FileIO::StoreAsPng(const std::string& src, const std::string& dst)
{
	if (!d2d::FileType::IsType(src, d2d::FileType::e_anim)) {
		return;
	}

	d2d::Snapshoot ss;
	d2d::Symbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(src);
	ss.OutputToImageFile(symbol, dst);
	symbol->Release();
}

Layer* FileIO::LoadLayer(const Json::Value& layerValue, const std::string& dir)
{
	Layer* layer = new Layer;
	InsertLayerSJ::Instance()->Insert(layer);
	SetSelectedSJ::Instance()->Set(0, 0);

	layer->SetName(layerValue["name"].asString());

	int i = 0;
	Json::Value frameValue = layerValue["frame"][i++];
	while (!frameValue.isNull()) {
		KeyFrame* frame = LoadFrame(layer, frameValue, dir);
		layer->InsertKeyFrame(frame);
		frame->Release();
		frameValue = layerValue["frame"][i++];
	}

	const std::map<int, KeyFrame*>& frames = layer->GetAllFrames();
	std::vector<KeyFrame*> all_frames;
	all_frames.reserve(frames.size());
	std::map<int, KeyFrame*>::const_iterator itr = frames.begin();
	for ( ; itr != frames.end(); ++itr)
		all_frames.push_back(itr->second);

	return layer;
}

KeyFrame* FileIO::LoadFrame(Layer* layer, const Json::Value& frameValue, const std::string& dir)
{
	int time = frameValue["time"].asInt();

	KeyFrame* frame = new KeyFrame(time);
	frame->SetLayer(layer);

	frame->SetID(frameValue["id"].asInt());

	frame->SetClassicTween(frameValue["tween"].asBool());

	int i = 0;
	Json::Value actorValue = frameValue["actor"][i++];
	while (!actorValue.isNull()) {
		d2d::Sprite* actor = LoadActor(actorValue, dir);
		frame->Insert(actor);
		actor->Release();
		actorValue = frameValue["actor"][i++];
	}

	LoadSkeleton(frameValue["skeleton"], frame->GetAllSprites(), frame->GetSkeletonData());

	return frame;
}

d2d::Sprite* FileIO::LoadActor(const Json::Value& actorValue, const std::string& dir)
{
	std::string filepath = actorValue["filepath"].asString();
	while (true) 
	{
		if (d2d::FileHelper::IsFileExist(filepath))
			break;

		std::string absolute_path = d2d::FileHelper::GetAbsolutePath(dir, filepath);
		if (d2d::FileHelper::IsFileExist(absolute_path))
		{
			filepath = absolute_path;
			break;
		}

		std::string res_path = absolute_path;
		if (!DataMgr::Instance()->GetTemplate().Empty())
			res_path = DataMgr::Instance()->GetTemplate().Dir() + "\\" + filepath;
		if (!d2d::FileHelper::IsFileExist(res_path))
			absolute_path = d2d::FileHelper::GetAbsolutePath(dir, res_path);
		if (!d2d::FileHelper::IsFileExist(absolute_path)) {
			throw d2d::Exception("File: %s don't exist!", res_path.c_str());
		}

		filepath = absolute_path;
		break;
	}

	d2d::Symbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
	d2d::SymbolSearcher::SetSymbolFilepaths(dir, symbol, actorValue);
//	symbol->refresh();
	d2d::Sprite* sprite = d2d::SpriteFactory::Instance()->Create(symbol);
	sprite->Load(actorValue);
	symbol->Release();

	return sprite;
}

void FileIO::LoadSkeleton(const Json::Value& skeletonValue, const std::vector<d2d::Sprite*>& sprites,
						  SkeletonData& skeleton)
{
	// prepare joints
	std::map<int, Joint*> mapJoint;
	
	int i = 0;
	Json::Value jointsVal = skeletonValue[i++];
	while (!jointsVal.isNull()) {
		std::string spriteName = jointsVal["sprite"].asString();
		d2d::Sprite* sprite = NULL;
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
		d2d::Sprite* sprite = NULL;
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
		skeleton.m_map_joints.insert(std::make_pair(sprite, joints));
		jointsVal = skeletonValue[i++];
	}
}

Layer* FileIO::LoadLayer(rapidxml::xml_node<>* layerNode,
						 const std::map<std::string, std::string>& mapNamePath)
{
	Layer* layer = new Layer;

	layer->SetName(layerNode->first_attribute("name")->value());

	rapidxml::xml_node<>* frameNode = layerNode->first_node("frames")
		->first_node("DOMFrame");
	while (frameNode) {
		KeyFrame* frame = LoadFrame(layer, frameNode, mapNamePath);
		layer->InsertKeyFrame(frame);
		frame->Release();
		frameNode = frameNode->next_sibling();
	}

	return layer;
}

KeyFrame* FileIO::LoadFrame(Layer* layer, rapidxml::xml_node<>* frameNode,
							const std::map<std::string, std::string>& mapNamePath)
{
	int time = d2d::StringHelper::FromString<int>(frameNode->first_attribute("index")->value()) + 1;

	KeyFrame* frame = new KeyFrame(time);
	frame->SetLayer(layer);

	rapidxml::xml_node<>* actorNode = frameNode->first_node("elements")
		->first_node("DOMSymbolInstance");
	while (actorNode) {
		d2d::Sprite* actor = LoadActor(actorNode, mapNamePath);
		frame->Insert(actor);
		actor->Release();
		actorNode = actorNode->next_sibling();
	}

	return frame;
}

d2d::Sprite* FileIO::LoadActor(rapidxml::xml_node<>* actorNode,
								const std::map<std::string, std::string>& mapNamePath)
{
	std::string name = actorNode->first_attribute("libraryItemName")->value();
	std::string filepath = mapNamePath.find(name)->second;
	d2d::Symbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
//	symbol->refresh();
	d2d::Sprite* sprite = d2d::SpriteFactory::Instance()->Create(symbol);
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

	sprite->SetTransform(d2d::Vector(float(tx+x), float(ty+y)), 0);

	return sprite;
}

Json::Value FileIO::StoreLayer(Layer* layer, const std::string& dir, 
							   bool single)
{
	Json::Value value;

	value["name"] = layer->GetName();

	const std::map<int, KeyFrame*>& frames = layer->GetAllFrames();
	std::vector<KeyFrame*> all_frames;
	all_frames.reserve(frames.size());
	std::map<int, KeyFrame*>::const_iterator itr = frames.begin();
	for (size_t i = 0; itr != frames.end(); ++itr, ++i) {
		value["frame"][i] = StoreFrame(itr->second, dir, single);
		all_frames.push_back(itr->second);
	}

	return value;
}

Json::Value FileIO::StoreFrame(KeyFrame* frame, const std::string& dir, 
							   bool single)
{
	Json::Value value;

	value["id"] = frame->GetID();

	value["time"] = frame->GetTime();

	value["tween"] = frame->HasClassicTween();

	for (size_t i = 0, n = frame->Size(); i < n; ++i)
		value["actor"][i] = StoreActor(frame->GetSprite(i), dir, single);

	value["skeleton"] = StoreSkeleton(frame->GetSkeletonData());

	return value;
}

Json::Value FileIO::StoreActor(const d2d::Sprite* sprite, const std::string& dir,
							   bool single)
{
	Json::Value value;

	const d2d::Symbol& symbol = sprite->GetSymbol();
	// filepath
	std::string relative_path = d2d::FileHelper::GetRelativePath(dir, 
		symbol.GetFilepath()).ToStdString();
	if (single) {
		value["filepath"] = relative_path;
	} else {
		if (DataMgr::Instance()->GetTemplate().ContainPath(relative_path)) {
			value["filepath"] = d2d::FileHelper::GetFilenameWithExtension(
				symbol.GetFilepath()).ToStdString();
		} else {
			value["filepath"] = relative_path;
		}
	}
	// filepaths
	const std::set<std::string>& filepaths = symbol.GetFilepaths();
	std::set<std::string>::const_iterator itr = filepaths.begin();
	for (int i = 0; itr != filepaths.end(); ++itr, ++i) {
		value["filepaths"][i] = *itr;
	}
	// other
	sprite->Store(value);

	return value;
}

Json::Value FileIO::StoreSkeleton(const SkeletonData& skeleton)
{
	Json::Value value;

	std::map<d2d::Sprite*, std::vector<Joint*> >::const_iterator itr
		= skeleton.m_map_joints.begin();
	for (int i = 0; itr != skeleton.m_map_joints.end(); ++itr, ++i)
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