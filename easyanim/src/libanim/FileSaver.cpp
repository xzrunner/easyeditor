#include "FileSaver.h"
#include "Symbol.h"

#include <ee/FileHelper.h>
#include <ee/Sprite.h>
#include <ee/SymbolPath.h>

#include <sprite2/Sprite.h>
#include <sprite2/ILerp.h>
#include <sprite2/LerpType.h>
#include <sprite2/LerpCircle.h>
#include <sprite2/LerpSpiral.h>
#include <sprite2/LerpWiggle.h>
#include <sprite2/LerpEase.h>

#include <fstream>

namespace libanim
{

void FileSaver::Store(const std::string& filepath, const Symbol& sym)
{
	Json::Value value;

	value["name"] = sym.name;

	value["fps"] = sym.GetFPS();

	const auto& layers = sym.GetLayers();
	std::string dir = ee::FileHelper::GetFileDir(filepath);
	for (size_t i = 0, n = layers.size(); i < n; ++i)
		Store(value["layer"][i], layers[i], dir);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

Json::Value FileSaver::StoreLerps(const CU_VEC<std::pair<s2::AnimLerp::SprData, std::unique_ptr<s2::ILerp>>>& lerps)
{
	Json::Value ret;
	for (int i = 0, n = lerps.size(); i < n; ++i) 
	{
		ret[i]["key"] = lerps[i].first;

		Json::Value val;
		const s2::ILerp& lerp = *lerps[i].second;
		switch (lerp.Type())
		{
		case s2::LERP_CIRCLE:
			{
				val["type"] = "circle";
				const s2::LerpCircle& circle = static_cast<const s2::LerpCircle&>(lerp);
				val["scale"] = static_cast<int>(circle.GetScale() * 100);
			}
			break;
		case s2::LERP_SPIRAL:
			{
				val["type"] = "spiral";
				const s2::LerpSpiral& spiral = static_cast<const s2::LerpSpiral&>(lerp);
				float begin, end;
				spiral.GetAngle(begin, end);
				val["angle_begin"] = static_cast<int>(begin * SM_RAD_TO_DEG);
				val["angle_end"]   = static_cast<int>(end * SM_RAD_TO_DEG);
				val["scale"] = static_cast<int>(spiral.GetScale() * 100);
			}
			break;
		case s2::LERP_WIGGLE:
			{
				val["type"] = "wiggle";
				const s2::LerpWiggle& wiggle = static_cast<const s2::LerpWiggle&>(lerp);
				val["freq"] = wiggle.GetFreq();
				val["amp"] = wiggle.GetAmp();
			}
			break;
		case s2::LERP_EASE:
			{
				val["type"] = "ease";
				const s2::LerpEase& ease = static_cast<const s2::LerpEase&>(lerp);
				val["ease"] = ease.GetEaseType();
			}
			break;
		}

		ret[i]["val"] = val;
	}
	return ret;
}

void FileSaver::Store(Json::Value& value, const s2::AnimSymbol::LayerPtr& layer, const std::string& dir)
{
	value["name"] = layer->name.c_str();
	for (size_t i = 0, n = layer->frames.size(); i < n; ++i) {
		Store(value["frame"][i], layer->frames[i], dir);
	}
}

void FileSaver::Store(Json::Value& value, const s2::AnimSymbol::FramePtr& frame, const std::string& dir)
{
	value["time"] = frame->index;
	value["tween"] = frame->tween;
	for (size_t i = 0, n = frame->sprs.size(); i < n; ++i) {
		Store(value["actor"][i], frame->sprs[i], dir);
	}
	value["lerp"] = StoreLerps(frame->lerps);
}

void FileSaver::Store(Json::Value& value, const s2::SprPtr& spr, const std::string& dir)
{
	auto ee_spr = std::dynamic_pointer_cast<ee::Sprite>(spr);
	auto ee_sym = std::dynamic_pointer_cast<ee::Symbol>(ee_spr->GetSymbol());

	// filepath
	value["filepath"] = ee::SymbolPath::GetRelativePath(*ee_sym, dir);
	// filepaths
	const std::set<std::string>& filepaths = ee_sym->GetFilepaths();
	std::set<std::string>::const_iterator itr = filepaths.begin();
	for (int i = 0; itr != filepaths.end(); ++itr, ++i) {
		value["filepaths"][i] = *itr;
	}
	// other
	ee_spr->Store(value, dir);
}

} // anim