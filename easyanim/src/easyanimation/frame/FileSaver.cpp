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
#include <sprite2/ILerp.h>
#include <sprite2/LerpType.h>
#include <sprite2/LerpCircle.h>
#include <sprite2/LerpSpiral.h>
#include <sprite2/LerpWiggle.h>
#include <sprite2/LerpEase.h>

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

	ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(src);
	libanim::Symbol* anim = static_cast<libanim::Symbol*>(sym);

	sm::vec2 sz = sym->GetBounding().Size();
	AnimatedGifSaver saver(sz.x, sz.y);
	s2::DrawRT rt(sz.x, sz.y);

	int max_frame = anim->GetMaxFrameIdx();
	for (int i = 0; i < max_frame; ++i)
	{
		rt.Draw(sym, true);
		uint8_t* rgb = rt.StoreToMemory(-1, -1, 3);
		//		anim->setFrameIndex(i + 1);
		saver.AddFrame(rgb, 1.0f / anim->GetFPS());
		delete[] rgb;
	}
	//	anim->setFrameIndex(0);
	saver.Save(dst.c_str());

	sym->RemoveReference();
}

void FileSaver::StoreAsPng(const std::string& src, const std::string& dst)
{
	if (ee::SymbolFile::Instance()->Type(src) != s2::SYM_ANIMATION) {
		return;
	}

	ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(src);

	sm::vec2 sz = sym->GetBounding().Size();
	s2::DrawRT rt(sz.x, sz.y);
	rt.Draw(sym);
	rt.StoreToFile(dst);

	sym->RemoveReference();
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
	value["lerp"] = StoreLerp(frame);

	for (size_t i = 0, n = frame->Size(); i < n; ++i)
		value["actor"][i] = StoreActor(frame->GetSprite(i), dir, single);

	value["skeleton"] = StoreSkeleton(frame->GetSkeletonData());

	return value;
}

Json::Value FileSaver::StoreLerp(KeyFrame* frame)
{
	Json::Value value;

	const std::vector<std::pair<int, s2::ILerp*> >& lerps = frame->GetLerps();
	for (int i = 0, n = lerps.size(); i < n; ++i) 
	{
		value[i]["key"] = lerps[i].first;

		Json::Value val;
		s2::ILerp* lerp = lerps[i].second;
		switch (lerp->Type())
		{
		case s2::LERP_CIRCLE:
			{
				val["type"] = "circle";
				s2::LerpCircle* circle = static_cast<s2::LerpCircle*>(lerp);
				val["scale"] = static_cast<int>(circle->GetScale() * 100);
			}
			break;
		case s2::LERP_SPIRAL:
			{
				val["type"] = "spiral";
				s2::LerpSpiral* spiral = static_cast<s2::LerpSpiral*>(lerp);
				float begin, end;
				spiral->GetAngle(begin, end);
				val["angle_begin"] = static_cast<int>(begin * SM_RAD_TO_DEG);
				val["angle_end"]   = static_cast<int>(end * SM_RAD_TO_DEG);
				val["scale"] = static_cast<int>(spiral->GetScale() * 100);
			}
			break;
		case s2::LERP_WIGGLE:
			{
				val["type"] = "wiggle";
				s2::LerpWiggle* wiggle = static_cast<s2::LerpWiggle*>(lerp);
				val["freq"] = wiggle->GetFreq();
				val["amp"] = wiggle->GetAmp();
			}
			break;
		case s2::LERP_EASE:
			{
				val["type"] = "ease";
				s2::LerpEase* ease = static_cast<s2::LerpEase*>(lerp);
				val["ease"] = ease->GetEaseType();
			}
			break;
		}

		value[i]["val"] = val;
	}

	return value;
}

Json::Value FileSaver::StoreActor(const ee::Sprite* spr, const std::string& dir,
								  bool single)
{
	Json::Value value;
	const ee::Symbol* sym = dynamic_cast<const ee::Symbol*>(spr->GetSymbol());
	// filepath
	std::string filepath = ee::SymbolPath::GetRelativePath(sym, dir);
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

	const std::map<ee::Sprite*, std::vector<Joint*> >& map_joints = skeleton.GetMapJoints();
	std::map<ee::Sprite*, std::vector<Joint*> >::const_iterator itr
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