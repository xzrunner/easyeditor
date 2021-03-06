#include "SetLayerTagAOP.h"
#include "typedef.h"

#include <ee/Sprite.h>
#include <ee/panel_msg.h>

namespace lr
{

SetLayerTagAOP::SetLayerTagAOP(Type type)
	: m_type(type)
{
}

void SetLayerTagAOP::Undo()
{
}

void SetLayerTagAOP::Redo()
{
}

void SetLayerTagAOP::Copy(const std::vector<SprPtr>& sprs)
{
	Set(sprs, m_type);
}

Json::Value SetLayerTagAOP::Store(const std::vector<ee::SprPtr>& sprs) const
{
	return Json::Value();
}

void SetLayerTagAOP::Set(const std::vector<ee::SprPtr>& sprs, Type type)
{
	for (int i = 0, n = sprs.size(); i < n; ++i)
	{
		auto& spr = sprs[i];

		std::string tag = spr->GetTag();
		size_t p_begin = tag.find("layer=");
		if (p_begin != std::string::npos) {
			size_t p_end = tag.find(";", p_begin) + 1;
			tag.erase(tag.begin() + p_begin, tag.begin() + p_end);
		}

		if (type == COVER) {
			tag += std::string(COVER_LAYER_TAG) + ";";
		} else if (type == TOP) {
			tag += std::string(TOP_LAYER_TAG) + ";";
		} else if (type == BG_DOWN) {
			tag += std::string(BG_DOWN_LAYER_TAG) + ";";
		} else if (type == BG_UP) {
			tag += std::string(BG_UP_LAYER_TAG) + ";";
		}
		spr->SetTag(tag);
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}