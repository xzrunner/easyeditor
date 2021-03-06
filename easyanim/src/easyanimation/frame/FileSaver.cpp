#include "FileSaver.h"
#include "dataset/DataMgr.h"
#include "dataset/Layer.h"
#include "dataset/KeyFrame.h"
#include "dataset/Joint.h"
#include "message/messages.h"

#include <ee/FileHelper.h>
#include <ee/SymbolFile.h>
#include <ee/Symbol.h>
#include <ee/SymbolMgr.h>
#include <ee/AnimatedGifSaver.h>
#include <ee/SymbolType.h>
#include <ee/SymbolPath.h>

#include <easyanim.h>

#include <sprite2/SymType.h>
#include <sprite2/DrawRT.h>

#include <fstream>

namespace eanim
{

void FileSaver::StoreSingle(const std::string& filepath)
{
	Json::Value value;

	value["name"] = DataMgr::Instance()->name;
	value["fps"] = GetFpsSJ::Instance()->Get();

	std::string dir = ee::FileHelper::GetFileDir(filepath);

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

void FileSaver::StoreTemplate(const std::string& filepath)
{
	Json::Value value;

	AnimTemplate& temp = DataMgr::Instance()->GetTemplate();
	temp.PreparePaths(filepath);
	temp.StoreToFile(value["template"]);

	value["name"] = DataMgr::Instance()->name;
	value["fps"] = GetFpsSJ::Instance()->Get();

	std::string dir = ee::FileHelper::GetFileDir(filepath);
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

void FileSaver::StoreAsGif(const std::string& src, const std::string& dst)
{
	if (ee::SymbolFile::Instance()->Type(src) != s2::SYM_ANIMATION) {
		return;
	}

	auto sym = ee::SymbolMgr::Instance()->FetchSymbol(src);
	auto anim = std::dynamic_pointer_cast<libanim::Symbol>(sym);

	sm::vec2 sz = sym->GetBounding().Size();
	AnimatedGifSaver saver(sz.x, sz.y);
	s2::DrawRT rt(sz.x, sz.y);

	int max_frame = anim->GetMaxFrameIdx();
	for (int i = 0; i < max_frame; ++i)
	{
		rt.Draw(*sym, true);
		uint8_t* rgb = rt.StoreToMemory(-1, -1, 3);
		//		anim->setFrameIndex(i + 1);
		saver.AddFrame(rgb, 1.0f / anim->GetFPS());
		delete[] rgb;
	}
	//	anim->setFrameIndex(0);
	saver.Save(dst.c_str());
}

void FileSaver::StoreAsPng(const std::string& src, const std::string& dst)
{
	if (ee::SymbolFile::Instance()->Type(src) != s2::SYM_ANIMATION) {
		return;
	}

	auto sym = ee::SymbolMgr::Instance()->FetchSymbol(src);

	sm::vec2 sz = sym->GetBounding().Size();
	s2::DrawRT rt(sz.x, sz.y);
	rt.Draw(*sym);
	rt.StoreToFile(dst.c_str(), sz.x, sz.y);
}

Json::Value FileSaver::StoreLayer(Layer* layer, const std::string& dir, 
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

Json::Value FileSaver::StoreFrame(KeyFrame* frame, const std::string& dir, 
								  bool single)
{
	Json::Value value;

	value["id"] = frame->GetID();

	value["time"] = frame->GetTime();

	value["tween"] = frame->HasClassicTween();
	value["lerp"] = libanim::FileSaver::StoreLerps(frame->GetLerps());

	for (size_t i = 0, n = frame->Size(); i < n; ++i)
		value["actor"][i] = StoreActor(frame->GetSprite(i), dir, single);

	value["skeleton"] = StoreSkeleton(frame->GetSkeletonData());

	return value;
}

Json::Value FileSaver::StoreActor(const ee::SprConstPtr& spr, const std::string& dir,
								  bool single)
{
	Json::Value value;
	auto sym = std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol());
	// filepath
	std::string filepath = ee::SymbolPath::GetRelativePath(*sym, dir);
	if (single) {
		value["filepath"] = filepath;
	} else {
		if (DataMgr::Instance()->GetTemplate().ContainPath(filepath)) {
			value["filepath"] = ee::FileHelper::GetFilenameWithExtension(
				sym->GetFilepath());
		} else {
			value["filepath"] = filepath;
		}
	}
	// filepaths
	const std::set<std::string>& filepaths = sym->GetFilepaths();
	std::set<std::string>::const_iterator itr = filepaths.begin();
	for (int i = 0; itr != filepaths.end(); ++itr, ++i) {
		value["filepaths"][i] = *itr;
	}
	// other
	spr->Store(value, dir);

	return value;
}

Json::Value FileSaver::StoreSkeleton(const SkeletonData& skeleton)
{
	Json::Value value;

	const std::map<ee::SprPtr, std::vector<Joint*> >& map_joints = skeleton.GetMapJoints();
	std::map<ee::SprPtr, std::vector<Joint*> >::const_iterator itr
		= map_joints.begin();
	for (int i = 0; itr != map_joints.end(); ++itr, ++i)
	{
		value[i]["sprite"] = itr->first->GetName();
		for (int j = 0, m = itr->second.size(); j < m; ++j)
		{
			Joint* joint = itr->second[j];

			Json::Value joint_val;
			joint_val["id"] = joint->GetID();
			joint_val["rx"] = joint->GetCurrRelativePos().x;
			joint_val["ry"] = joint->GetCurrRelativePos().y;
			if (const Joint* parent = joint->GetParent()) {
				joint_val["parent"] = parent->GetID();
			}

			const std::set<Joint*>& children = joint->GetChildren();
			std::set<Joint*>::const_iterator itr_child = children.begin();
			for (int k = 0; itr_child != children.end(); ++itr_child, ++k) {
				joint_val["children"][k] = (*itr_child)->GetID();
			}

			value[i]["joints"][j] = joint_val;
		}
	}

	return value;
}

}