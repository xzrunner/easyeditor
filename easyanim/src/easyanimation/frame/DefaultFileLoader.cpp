#include "DefaultFileLoader.h"
#include "message/messages.h"
#include "dataset/DataMgr.h"
#include "dataset/Layer.h"
#include "dataset/KeyFrame.h"
#include "dataset/Joint.h"
#include "view/ViewMgr.h"
#include "view/ToolbarPanel.h"
#include "view/LibraryPanel.h"

#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/Sprite.h>
#include <ee/SymbolType.h>
#include <ee/Exception.h>
#include <ee/SpriteFactory.h>

#include <sprite2/ILerp.h>
#include <sprite2/LerpType.h>
#include <sprite2/LerpCircle.h>
#include <sprite2/LerpSpiral.h>
#include <sprite2/LerpWiggle.h>
#include <sprite2/LerpEase.h>

namespace eanim
{

void DefaultFileLoader::Load(const Json::Value& val, const std::string& dir)
{
	int fps = val["fps"].asInt();
	SetFpsSJ::Instance()->Set(fps);

	if (val["template"].isNull()) {
		ViewMgr::Instance()->toolbar->ChangeTemplateMode(true);
	} else {
		DataMgr::Instance()->GetTemplate().LoadFromFile(val["template"]);
		ViewMgr::Instance()->toolbar->ChangeTemplateMode(false);
	}

	DataMgr::Instance()->name = val["name"].asString();

	DataMgr::Instance()->GetLayers().Clear();
	ViewMgr::Instance()->library->Clear();
	SetSelectedSJ::Instance()->Set(-1, -1);

	int i = 0;
	Json::Value layer_val = val["layer"][i++];
	while (!layer_val.isNull()) {
		LoadLayer(layer_val, dir);
		layer_val = val["layer"][i++];
	}

	ViewMgr::Instance()->library->LoadFromSymbolMgr(*ee::SymbolMgr::Instance());

	SetSelectedSJ::Instance()->Set(0, 0);
}

void DefaultFileLoader::Reload(const Json::Value& val, const std::string& dir)
{
	int i = 0;
	Json::Value layer_val = val["layer"][i++];
	while (!layer_val.isNull()) {
		Layer* layer = LoadLayer(layer_val, dir);
		layer_val = val["layer"][i++];
	}

	ViewMgr::Instance()->library->LoadFromSymbolMgr(*ee::SymbolMgr::Instance());
}

Layer* DefaultFileLoader::LoadLayer(const Json::Value& layer_val, const std::string& dir)
{
	Layer* layer = new Layer;
	InsertLayerSJ::Instance()->Insert(layer);
	SetSelectedSJ::Instance()->Set(0, 0);

	layer->SetName(layer_val["name"].asString());

	int i = 0;
	Json::Value frameValue = layer_val["frame"][i++];
	while (!frameValue.isNull()) {
		KeyFrame* frame = LoadFrame(layer, frameValue, dir);
		layer->InsertKeyFrame(frame);
		frame->RemoveReference();
		frameValue = layer_val["frame"][i++];
	}

	const std::map<int, KeyFrame*>& frames = layer->GetAllFrames();
	std::vector<KeyFrame*> all_frames;
	all_frames.reserve(frames.size());
	std::map<int, KeyFrame*>::const_iterator itr = frames.begin();
	for ( ; itr != frames.end(); ++itr)
		all_frames.push_back(itr->second);

	return layer;
}

KeyFrame* DefaultFileLoader::LoadFrame(Layer* layer, const Json::Value& frame_val, const std::string& dir)
{
	int time = frame_val["time"].asInt();

	KeyFrame* frame = new KeyFrame(time);
	frame->SetLayer(layer);

	frame->SetID(frame_val["id"].asInt());

	frame->SetClassicTween(frame_val["tween"].asBool());
	LoadLerp(frame, frame_val["lerp"]);

	int i = 0;
	Json::Value actorValue = frame_val["actor"][i++];
	while (!actorValue.isNull()) {
		ee::Sprite* actor = LoadActor(actorValue, dir);
		frame->Insert(actor, INT_MAX);
		actor->RemoveReference();
		actorValue = frame_val["actor"][i++];
	}

	LoadSkeleton(frame_val["skeleton"], frame->GetAllSprites(), frame->GetSkeletonData());

	return frame;
}

void DefaultFileLoader::LoadLerp(KeyFrame* frame, const Json::Value& value)
{
	for (int i = 0, n = value.size(); i < n; ++i)
	{
		int key = value[i]["key"].asInt();
		const Json::Value& val = value[i]["val"];
		if (val["type"].asString() == "circle")
		{
			float scale = val["scale"].asInt() * 0.01f;
			s2::LerpCircle* spiral = new s2::LerpCircle(scale);
			frame->SetLerp(key, spiral);
		}
		else if (val["type"].asString() == "spiral") 
		{
			float begin = val["angle_begin"].asInt() * SM_DEG_TO_RAD,
				end   = val["angle_end"].asInt() * SM_DEG_TO_RAD;
			float scale = val["scale"].asInt() * 0.01f;
			s2::LerpSpiral* spiral = new s2::LerpSpiral(begin, end, scale);
			frame->SetLerp(key, spiral);
		}
		else if (val["type"].asString() == "wiggle")
		{
			float freq = val["freq"].asDouble();
			float amp = val["amp"].asDouble();
			s2::LerpWiggle* wiggle = new s2::LerpWiggle(freq, amp);
			frame->SetLerp(key, wiggle);
		}
		else if (val["type"].asString() == "ease")
		{
			int type = val["ease"].asInt();
			s2::LerpEase* ease = new s2::LerpEase(type);
			frame->SetLerp(key, ease);
		}
	}
}

ee::Sprite* DefaultFileLoader::LoadActor(const Json::Value& val, const std::string& dir)
{
	std::string filepath = val["filepath"].asString();
	while (true) 
	{
		if (ee::FileHelper::IsFileExist(filepath)) {
			break;
		}

		if (filepath == ee::SYM_GROUP_TAG ||
			filepath == ee::SYM_SHAPE_TAG ||
			filepath == ee::SYM_TEXT_TAG) {
			break;
		}

		std::string absolute_path = ee::FileHelper::GetAbsolutePath(dir, filepath);
		if (ee::FileHelper::IsFileExist(absolute_path))
		{
			filepath = absolute_path;
			break;
		}

		std::string res_path = absolute_path;
		if (!DataMgr::Instance()->GetTemplate().Empty())
			res_path = DataMgr::Instance()->GetTemplate().Dir() + "\\" + filepath;
		if (!ee::FileHelper::IsFileExist(res_path))
			absolute_path = ee::FileHelper::GetAbsolutePath(dir, res_path);
		if (!ee::FileHelper::IsFileExist(absolute_path)) {
			throw ee::Exception("File: %s don't exist!", res_path.c_str());
		}

		filepath = absolute_path;
		break;
	}
	return ee::SpriteFactory::Instance()->Create(val, dir, filepath);
}

void DefaultFileLoader::LoadSkeleton(const Json::Value& skeleton_val, 
									 const std::vector<ee::Sprite*>& sprs,
									 SkeletonData& skeleton)
{
	// prepare joints
	std::map<int, Joint*> map_joint;

	int i = 0;
	Json::Value joints_val = skeleton_val[i++];
	while (!joints_val.isNull()) {
		std::string spriteName = joints_val["sprite"].asString();
		ee::Sprite* spr = NULL;
		for (int i = 0, n = sprs.size(); i < n; ++i)
		{
			if (sprs[i]->GetName() == spriteName) 
			{
				spr = sprs[i];
				break;
			}
		}
		assert(spr);

		int j = 0;
		Json::Value jointVal = joints_val["joints"][j++];
		while (!jointVal.isNull()) 
		{
			Joint* joint = new Joint(spr);

			int id = jointVal["id"].asInt();
			joint->SetID(id);

			sm::vec2 pos;
			pos.x = jointVal["rx"].asDouble();
			pos.y = jointVal["ry"].asDouble();			
			joint->SetRelativePos(pos);

			map_joint.insert(std::make_pair(id, joint));

			jointVal = joints_val["joints"][j++];
		}
		joints_val = skeleton_val[i++];
	}

	// skeleton
	i = 0;
	joints_val = skeleton_val[i++];
	while (!joints_val.isNull()) {
		std::string spriteName = joints_val["sprite"].asString();
		ee::Sprite* spr = NULL;
		for (int i = 0, n = sprs.size(); i < n; ++i)
		{
			if (sprs[i]->GetName() == spriteName) 
			{
				spr = sprs[i];
				break;
			}
		}
		assert(spr);

		std::vector<Joint*> joints;

		int j = 0;
		Json::Value jointVal = joints_val["joints"][j++];
		while (!jointVal.isNull()) {
			int joint_id = jointVal["id"].asInt();
			std::map<int, Joint*>::iterator itr = map_joint.find(joint_id);
			assert(itr != map_joint.end());
			Joint* joint = itr->second;
			if (!jointVal["parent"].isNull()) {
				itr = map_joint.find(jointVal["parent"].asInt());
				assert(itr != map_joint.end());
				joint->SetParent(itr->second);
			}
			if (!jointVal["children"].isNull()) {
				int k = 0;
				Json::Value childVal = jointVal["children"][k++];
				while (!childVal.isNull()) {
					itr = map_joint.find(childVal.asInt());
					assert(itr != map_joint.end());
					joint->AddChild(itr->second);
					childVal = jointVal["children"][k++];
				}
			}
			joints.push_back(joint);
			jointVal = joints_val["joints"][j++];
		}
		skeleton.InsertJoints(spr, joints);
		joints_val = skeleton_val[i++];
	}
}

}