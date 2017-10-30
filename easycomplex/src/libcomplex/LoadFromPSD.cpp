#include "LoadFromPSD.h"
#include "Symbol.h"
#include "Sprite.h"

#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/SpriteFactory.h>
#include <ee/Sprite.h>
#include <ee/SymbolType.h>

namespace ecomplex
{

void LoadFromPSD::Load(const Json::Value& value, const std::string& dir, Symbol& complex)
{
	int w = value["document"]["width"].asInt(),
		h = value["document"]["height"].asInt();

	const Json::Value& cvalue = value["children"];
//	for (int i = 0, n = cvalue.size(); i < n; ++i) {
	for (int i = cvalue.size() - 1; i >= 0; --i) {
		auto spr = LoadItem(cvalue[i], dir, w, h);
		if (spr) {
			complex.Add(spr);
		}
	}
}

ee::SprPtr LoadFromPSD::LoadItem(const Json::Value& value, const std::string& dir, int tw, int th)
{
	std::string type = value["type"].asString();
	if (type == "layer") {
		return LoadLayer(value, dir, tw, th);
	} else if (type == ee::SYM_GROUP_TAG) {
		return LoadGroup(value, dir, tw, th);
	} else {
		return NULL;
	}
}

ee::SprPtr LoadFromPSD::LoadGroup(const Json::Value& value, const std::string& dir, int tw, int th)
{
	std::string name = value[ee::SYM_GROUP_TAG].asString();

	bool visible = value["visible"].asBool();

	std::string _dir = dir + "\\" + name;
	
	auto sym = std::make_shared<Symbol>();
	auto spr = std::make_shared<Sprite>(sym);

	const Json::Value& cval = value["children"];
//	for (int i = 0, n = cval.size(); i < n; ++i) {
	for (int i = cval.size() - 1; i >= 0; --i) {
		ee::SprPtr cspr = LoadItem(cval[i], _dir, tw, th);
		if (cspr) {
			sym->Add(cspr);
		}
	}

//	sym->InitBounding();
	spr->UpdateBounding();

	spr->SetVisible(visible);

	return spr;
}

ee::SprPtr LoadFromPSD::LoadLayer(const Json::Value& value, const std::string& dir, int tw, int th)
{
	std::string name = value["name"].asString();

	bool visible = value["visible"].asBool();

	int left  = value["left"].asInt(),
		top   = value["top"].asInt();
	int width = value["width"].asInt(),
		height = value["height"].asInt();
	if (width == 0 || height == 0) {
		return NULL;
	}

	std::string filepath = ee::FileHelper::GetAbsolutePath(dir, name) + ".png";
	auto sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	auto spr = ee::SpriteFactory::Instance()->Create(sym);

	spr->SetPosition(sm::vec2(left + width * 0.5f - tw * 0.5f, th * 0.5f - top - height * 0.5f));
	spr->SetVisible(visible);

	return spr;
}

}