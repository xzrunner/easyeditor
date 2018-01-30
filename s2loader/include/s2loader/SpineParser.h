#ifndef _S2LOADER_SPINE_PARSER_H_
#define _S2LOADER_SPINE_PARSER_H_

#include <SM_Vector.h>
#include <cu/cu_stl.h>

#include <json/json.h>

namespace s2loader
{

class SpineParser
{
public:
	SpineParser(bool parse_anim = true) 
		: m_parse_anim(parse_anim) {}

	void Parse(const Json::Value& val);

public:
	struct Bone
	{
		CU_STR name;
		CU_STR parent;
		sm::vec2	pos;
		float		angle;
		sm::vec2	scale;
		float		length;

		Bone() : angle(0), scale(1, 1), length(0) {}
	};

	struct Slot
	{
		CU_STR name;
		CU_STR bone;
		CU_STR attachment;
	};

	struct IK
	{
		CU_STR name;
		CU_VEC<CU_STR> bones;
		CU_STR target;
		bool bend_positive;
	};

	enum SKIN_TYPE
	{
		SKIN_UNKNOWN = 0,
		SKIN_IMAGE,
		SKIN_MESH,
		SKIN_BOUNDINGBOX
	};

	struct SkinItem
	{
		CU_STR name;
		CU_STR path;

		SKIN_TYPE   type;

		int         width;
		int         height;

		// img
		sm::vec2 pos;
		float angle;

		struct SkinnedVertex
		{
			struct Joint
			{
				int bone;
				float vx, vy;
				float weight;
			};

			CU_VEC<Joint> joints;
		};

		// mesh
		CU_VEC<sm::vec2> vertices;
		CU_VEC<SkinnedVertex> skinned_vertices;
		CU_VEC<sm::vec2> texcoords;
		CU_VEC<int> triangles;
	};

	struct Skin
	{
		CU_STR name;
		CU_VEC<SkinItem> items;
	};

	struct Curve
	{
		bool operator == (const Curve& curve) const;

		float x0, y0;
		float x1, y1;
	};

	struct Rotate
	{
		float time;
		float rot;
		int   curve;

		Rotate() : time(0), rot(0), curve(-1) {}
	};

	struct Translate
	{
		float    time;
		sm::vec2 trans;
		int      curve;

		Translate() : time(0), curve(-1) {}
	};

	struct Scale
	{
		float    time;
		sm::vec2 scale;
		int      curve;

		Scale() : time(0), scale(1, 1), curve(-1) {}
	};

	struct AnimBone
	{
		CU_STR name;
		CU_VEC<Rotate> rotates;
		CU_VEC<Translate> translates;
		CU_VEC<Scale> scales;
	};

	struct AnimSlot
	{
		CU_STR name;
		CU_VEC<std::pair<float, CU_STR> > skins;
	};

	struct Deform
	{
		float time;
		int   offset;
		int   curve;
		CU_VEC<sm::vec2> vertices;
	};

	struct AnimDeform
	{
		CU_STR slot;
		CU_STR skin;
		CU_VEC<Deform> samples;
	};

	struct Animation
	{

		CU_STR name;
		CU_VEC<AnimBone> bones;
		CU_VEC<AnimSlot> slots;
		CU_VEC<AnimDeform> deforms;
	};

	const SkinItem* QuerySkin(const Slot& slot) const;

private:
	void Clear();

	void ParseHeader(const Json::Value& val);
	void ParseBones(const Json::Value& val);
	void ParseSlots(const Json::Value& val);
	void ParseIK(const Json::Value& val);
	void ParseSkins(const Json::Value& val);
	void ParseImage(SkinItem& dst, const Json::Value& src);
	void ParseMesh(SkinItem& dst, const Json::Value& src);
	
	void ParseAnimations(const Json::Value& val);
	void ParseAnimBond(const Json::Value& val, AnimBone& bone);
	void ParseAnimSlot(const Json::Value& val, AnimSlot& slot);
	void ParseAnimDeforms(const Json::Value& val, CU_VEC<AnimDeform>& deforms);
	void ParseAnimDeform(const Json::Value& val, AnimDeform& deform);

	int ParseCurve(const Json::Value& val);

public:
	bool m_parse_anim;

	CU_STR img_dir;

	CU_VEC<Bone> bones;

	CU_VEC<Slot> slots;

	CU_VEC<IK> iks;

	CU_VEC<Skin> skins;

	CU_VEC<Animation> anims;

	CU_VEC<Curve> curves;

}; // SpineParser

}

#endif // _S2LOADER_SPINE_PARSER_H_
