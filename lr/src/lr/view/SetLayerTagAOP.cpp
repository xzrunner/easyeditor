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

void SetLayerTagAOP::Copy(const std::vector<ee::Sprite*>& sprs)
{
	Set(sprs, m_type);
}

Json::Value SetLayerTagAOP::Store(const std::vector<ee::Sprite*>& sprs) const
{
	return Json::Value();
}

void SetLayerTagAOP::Set(const std::vector<ee::Sprite*>& sprs, Type type)
{
	for (int i = 0, n = sprs.size(); i < n; ++i)
	{
		ee::Sprite* spr = sprs[i];

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
		}
		spr->SetTag(tag);
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}