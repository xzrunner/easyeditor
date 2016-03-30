#include "RenderShader.h"
#include "Config.h"

namespace ee
{

RenderShader::RenderShader()
	: blend(BM_NULL)
	, filter(FM_NULL)
{
}

void RenderShader::LoadFromFile(const Json::Value& val)
{
	if (!val["blend"].isNull()) {
		std::string disc = val["blend"].asString();
		if (Config::Instance()->IsRenderOpen()) {
			blend = BlendModes::Instance()->GetModeFromNameEN(disc);
		}
	}

	if (!val["filter"].isNull()) {
		std::string disc = val["filter"].asString();
		if (Config::Instance()->IsRenderOpen()) {
			filter = FilterModes::Instance()->GetIDFromNameEN(disc);
		}
	}
}

void RenderShader::StoreToFile(Json::Value& val) const
{
	if (Config::Instance()->IsRenderOpen()) {
		val["blend"] = BlendModes::Instance()->GetNameENFromMode(blend);
		val["filter"] = FilterModes::Instance()->GetNameENFromID(filter);
	}
}

}