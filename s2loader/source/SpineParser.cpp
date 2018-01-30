#include "s2loader/SpineParser.h"

#include <sm_const.h>

#include <fstream>

#include <assert.h>

namespace s2loader
{

void SpineParser::Parse(const Json::Value& val)
{
	Clear();

	ParseHeader(val["skeleton"]);
	ParseBones(val["bones"]);
	ParseSlots(val["slots"]);
	if (val.isMember("ik")) {
		ParseIK(val["ik"]);
	}
	ParseSkins(val["skins"]["default"]);
	Json::Value::Members key_skins = val["skins"].getMemberNames();
	for (int i = 0, n = key_skins.size(); i < n; ++i) {
		const std::string& key = key_skins[i];
		if (key == "default") {
			continue;
		} else {
			ParseSkins(val["skins"][key]);
			break;
		}
	}
	if (m_parse_anim) {
		ParseAnimations(val["animations"]);
	}
}

const SpineParser::SkinItem* SpineParser::QuerySkin(const Slot& slot) const
{
	const Skin* skin = nullptr;
	for (int i = 0, n = skins.size(); i < n; ++i) {
		if (skins[i].name == slot.name) {
			skin = &skins[i];
			break;
		}
	}
	assert(skin);

	for (int i = 0, n = skin->items.size(); i < n; ++i) {
		const SkinItem& item = skin->items[i];
		if (slot.attachment == item.name && item.type != SKIN_BOUNDINGBOX) {
			return &item;
		}
	}

	return nullptr;
}

void SpineParser::Clear()
{
	bones.clear();
	slots.clear();
	skins.clear();
}

void SpineParser::ParseHeader(const Json::Value& val)
{
	img_dir = "." + CU_STR(val["images"].asString().c_str());
}

void SpineParser::ParseBones(const Json::Value& val)
{
	bones.reserve(val.size());
	for (int i = 0, n = val.size(); i < n; ++i)
	{
		const Json::Value& src = val[i];
		Bone dst;
		dst.name = src["name"].asString().c_str();
		dst.parent = src["parent"].asString().c_str();
		dst.pos.x = static_cast<float>(src["x"].asDouble());
		dst.pos.y = static_cast<float>(src["y"].asDouble());
		dst.angle = static_cast<float>(src["rotation"].asDouble() * SM_DEG_TO_RAD);
		if (src.isMember("length")) {
			dst.length = static_cast<float>(src["length"].asDouble());
		}

		bones.push_back(dst);
	}
}

void SpineParser::ParseSlots(const Json::Value& val)
{
	for (int i = 0, n = val.size(); i < n; ++i)
	{
		const Json::Value& src = val[i];
		Slot dst;
		dst.name = src["name"].asString().c_str();
		dst.bone = src["bone"].asString().c_str();
		dst.attachment = src["attachment"].asString().c_str();
		slots.push_back(dst);
	}
}

void SpineParser::ParseIK(const Json::Value& val)
{
	for (int i = 0, n = val.size(); i < n; ++i)
	{
		const Json::Value& src = val[i];
		IK dst;
		dst.name = src["name"].asString().c_str();
		dst.bones.reserve(src["bones"].size());
		for (int j = 0, m = src["bones"].size(); j < m; ++j) {
			dst.bones.push_back(src["bones"][j].asString().c_str());
		}
		dst.target = src["target"].asString().c_str();
		if (src.isMember("bendPositive")) {
			dst.bend_positive = src["bendPositive"].asBool();
		} else {
			dst.bend_positive = true;
		}
		iks.push_back(dst);
	}
}

void SpineParser::ParseSkins(const Json::Value& val)
{
	Json::Value::Members key_skins = val.getMemberNames();
	for (int i = 0, n = key_skins.size(); i < n; ++i)
	{
		const std::string& key = key_skins[i];
		const Json::Value& src_skin = val[key];
		Skin skin;
		skin.name = key.c_str();
		Json::Value::Members key_items = src_skin.getMemberNames();
		for (int j = 0, m = key_items.size(); j < m; ++j)
		{
			const std::string& key = key_items[j];
			const Json::Value& src_item = src_skin[key];
			SkinItem item;
			item.name = key.c_str();
			if (src_item.isMember("name")) {
				item.path = src_item["name"].asString().c_str();
			} else {
				item.path = key.c_str();
			}

			item.type = SKIN_IMAGE;
			if (src_item.isMember("type")) {
				CU_STR stype = src_item["type"].asString().c_str();
				if (stype == "boundingbox") {
					item.type = SKIN_BOUNDINGBOX;
				} else if (stype == "mesh") {
					item.type = SKIN_MESH;
				} else {
					item.type = SKIN_UNKNOWN;
				}
			}

			item.width = src_item["width"].asInt();
			item.height = src_item["height"].asInt();

			switch (item.type)
			{
			case SKIN_IMAGE:
				ParseImage(item, src_item);
				break;
			case SKIN_MESH:
				ParseMesh(item, src_item);
				break;
			default:
				break;
			}

			skin.items.push_back(item);
		}
		skins.push_back(skin);
	}
}

void SpineParser::ParseImage(SkinItem& dst, const Json::Value& src)
{
	dst.pos.x = static_cast<float>(src["x"].asDouble());
	dst.pos.y = static_cast<float>(src["y"].asDouble());
	dst.angle = static_cast<float>(src["rotation"].asDouble() * SM_DEG_TO_RAD);
}

void SpineParser::ParseMesh(SkinItem& dst, const Json::Value& src)
{
	int ptr = 0;
	int tn = src["uvs"].size() / 2;
	dst.texcoords.reserve(tn);
	for (int i = 0; i < tn; ++i) 
	{
		sm::vec2 pos;
		pos.x = static_cast<float>(src["uvs"][ptr++].asDouble());
		pos.y = static_cast<float>(src["uvs"][ptr++].asDouble());
		pos.y = 1 - pos.y;
		dst.texcoords.push_back(pos);
	}

	if (src["vertices"].size() == src["uvs"].size())
	{
		int ptr = 0;
		int vn = tn;
		dst.vertices.reserve(vn);
		for (int i = 0; i < vn; ++i) 
		{
			sm::vec2 pos;
			pos.x = static_cast<float>(src["vertices"][ptr++].asDouble());
			pos.y = static_cast<float>(src["vertices"][ptr++].asDouble());
			dst.vertices.push_back(pos);
		}
		assert(dst.vertices.size() == dst.texcoords.size());
	}
	else
	{
		for (int i = 0, n = src["vertices"].size(); i < n; )
		{
			SkinItem::SkinnedVertex sv;
			int bone_n = src["vertices"][i++].asInt();
			sv.joints.reserve(bone_n);
			for (int j = 0; j < bone_n; ++j) 
			{
				SkinItem::SkinnedVertex::Joint item;
				item.bone = src["vertices"][i++].asInt();
				item.vx = static_cast<float>(src["vertices"][i++].asDouble());
				item.vy = static_cast<float>(src["vertices"][i++].asDouble());
				item.weight = static_cast<float>(src["vertices"][i++].asDouble());
				sv.joints.push_back(item);
			}
			dst.skinned_vertices.push_back(sv);
		}
		assert(dst.skinned_vertices.size() == dst.texcoords.size());
	}

	ptr = 0;
	dst.triangles.reserve(src["triangles"].size());
	for (int i = 0, n = src["triangles"].size(); i < n; ++i) {
		dst.triangles.push_back(src["triangles"][ptr++].asInt());
	}
}

void SpineParser::ParseAnimations(const Json::Value& val)
{
	Json::Value::Members key_anis = val.getMemberNames();
	anims.reserve(key_anis.size());
	for (int i = 0, n = key_anis.size(); i < n; ++i)
	{
		const std::string& key = key_anis[i];
		const Json::Value& src = val[key];
		Animation dst;
		dst.name = key.c_str();

		Json::Value::Members key_bones = src["bones"].getMemberNames();
		dst.bones.reserve(key_bones.size());
		for (int j = 0, m = key_bones.size(); j < m; ++j)
		{
			const std::string& key = key_bones[j];
			AnimBone bone;
			bone.name = key.c_str();
			ParseAnimBond(src["bones"][key], bone);
			dst.bones.push_back(bone);
		}

		Json::Value::Members key_slots = src["slots"].getMemberNames();
		dst.slots.reserve(key_slots.size());
		for (int j = 0, m = key_slots.size(); j < m; ++j)
		{
			const std::string& key = key_slots[j];
			AnimSlot slot;
			slot.name = key.c_str();
			ParseAnimSlot(src["slots"][key]["attachment"], slot);
			dst.slots.push_back(slot);
		}

		Json::Value::Members key_deform = src["deform"].getMemberNames();
		if (key_deform.size() > 0)
		{
			ParseAnimDeforms(src["deform"]["default"], dst.deforms);
			if (key_deform.size() > 1) {
				const std::string& key = key_deform[1];
				ParseAnimDeforms(src["deform"][key], dst.deforms);
			}
		}

		anims.push_back(dst);
	}
}

void SpineParser::ParseAnimBond(const Json::Value& val, AnimBone& bone)
{
	if (val.isMember("rotate"))
	{
		bone.rotates.reserve(val["rotate"].size());
		for (int i = 0, n = val["rotate"].size(); i < n; ++i)
		{
			const Json::Value& src = val["rotate"][i];
			Rotate dst;
			dst.time  = static_cast<float>(src["time"].asDouble());
			dst.rot   = static_cast<float>(src["angle"].asDouble() * SM_DEG_TO_RAD);
			dst.curve = ParseCurve(src["curve"]);
			bone.rotates.push_back(dst);
		}
	}
	if (val.isMember("translate"))
	{
		bone.translates.reserve(val["translate"].size());
		for (int i = 0, n = val["translate"].size(); i < n; ++i)
		{
			const Json::Value& src = val["translate"][i];
			Translate dst;
			dst.time    = static_cast<float>(src["time"].asDouble());
			dst.trans.x = static_cast<float>(src["x"].asDouble());
			dst.trans.y = static_cast<float>(src["y"].asDouble());
			dst.curve   = ParseCurve(src["curve"]);
			bone.translates.push_back(dst);
		}
	}
	if (val.isMember("scale"))
	{
		bone.scales.reserve(val["scale"].size());
		for (int i = 0, n = val["scale"].size(); i < n; ++i)
		{
			const Json::Value& src = val["scale"][i];
			Scale dst;
			dst.time    = static_cast<float>(src["time"].asDouble());
			dst.scale.x = static_cast<float>(src["x"].asDouble());
			dst.scale.y = static_cast<float>(src["y"].asDouble());
			dst.curve   = ParseCurve(src["curve"]);
			bone.scales.push_back(dst);
		}
	}
}

void SpineParser::ParseAnimSlot(const Json::Value& val, AnimSlot& slot)
{
	slot.skins.reserve(val.size());
	for (int i = 0, n = val.size(); i < n; ++i) {
		float time = static_cast<float>(val[i]["time"].asDouble());
		CU_STR skin = val[i]["name"].asString().c_str();
		slot.skins.push_back(std::make_pair(time, skin));
	}
}

void SpineParser::ParseAnimDeforms(const Json::Value& val, CU_VEC<AnimDeform>& deforms)
{
	Json::Value::Members key_slots = val.getMemberNames();
	for (int i = 0, n = key_slots.size(); i < n; ++i)
	{
		const std::string& slot = key_slots[i];
		Json::Value::Members key_skins = val[slot].getMemberNames();
		for (int j = 0, m = key_skins.size(); j < m; ++j)
		{
			const std::string& skin = key_skins[j];
			AnimDeform deform;
			deform.slot = slot.c_str();
			deform.skin = skin.c_str();
			ParseAnimDeform(val[slot][skin], deform);
			deforms.push_back(deform);
		}
	}
}

void SpineParser::ParseAnimDeform(const Json::Value& val, AnimDeform& deform)
{
	deform.samples.reserve(val.size());
	for (int i = 0, n = val.size(); i < n; ++i) 
	{
		const Json::Value& src = val[i];
		Deform dst;

		dst.time = static_cast<float>(src["time"].asDouble());
		if (src.isMember("offset")) {
			dst.offset = src["offset"].asInt() / 2;
		} else {
			dst.offset = 0;
		}

		dst.curve = ParseCurve(src["curve"]);

		int ptr = 0;
		int m = src["vertices"].size() / 2;
		dst.vertices.reserve(m);
		for (int j = 0; j < m; ++j) {
			float x = static_cast<float>(src["vertices"][ptr++].asDouble()),
				  y = static_cast<float>(src["vertices"][ptr++].asDouble());
			dst.vertices.push_back(sm::vec2(x, y));
		}

		deform.samples.push_back(dst);
	}
}

int SpineParser::ParseCurve(const Json::Value& val)
{
	if (val.isNull() || !val.isArray() || val.size() != 4) {
		return -1;
	}
	Curve curve;
	int idx0 = 0;
	curve.x0 = val[idx0].asDouble();
	curve.y0 = val[1].asDouble();
	curve.x1 = val[2].asDouble();
	curve.y1 = val[3].asDouble();
	
	for (int i = 0, n = curves.size(); i < n; ++i) {
		if (curves[i] == curve) {
			return i;
		}
	}
	curves.push_back(curve);
	return curves.size() - 1;
}

bool SpineParser::Curve::operator == (const Curve& curve) const
{
	return
		x0 == curve.x0 &&
		y0 == curve.y0 &&
		x1 == curve.x1 &&
		y1 == curve.y1;
}

}