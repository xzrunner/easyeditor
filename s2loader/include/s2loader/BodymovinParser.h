#ifndef _S2LOADER_BODYMOVIN_PARSER_H_
#define _S2LOADER_BODYMOVIN_PARSER_H_

#include <SM_Vector.h>
#include <cu/cu_stl.h>

#include <json/json.h>

#include <string.h>

namespace s2loader
{

class BodymovinParser
{
public:
	BodymovinParser();

	void Parse(const Json::Value& val, const CU_STR& dir);

	int GetFrameRate() const { return m_frame_rate; }

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

	int GetStartFrame() const { return m_start_frame; }
	int GetEndFrame() const { return m_end_frame; }

public:
	struct Layer;
	struct Asset
	{
		CU_STR id;

		// single
		CU_STR filepath;
		int w, h;

		// compose
		CU_VEC<Layer> layers;
	};

	enum LayerType 
	{
		LAYER_PRE_COMP,
		LAYER_SOLID,
		LAYER_IMAGE,
		LAYER_NULL,
		LAYER_SHAPE,
		LAYER_TEXT,
		LAYER_UNKNOWN
	};

	struct FloatVal
	{
		struct Float3
		{
			float data[3];

			Float3();
			Float3(const Json::Value& val);

			bool operator == (const Float3& f) const;
		};

		struct KeyFrame
		{
			int frame;
			Float3 s_val, e_val;
			Float3 ix, iy, ox, oy;
			Float3 ti, to;

			KeyFrame() { memset(this, 0, sizeof(*this)); }
		};

		CU_VEC<KeyFrame> frames;

		CU_STR expression;

		void Load(const Json::Value& val);		
	};

	struct Transform
	{
		FloatVal anchor;
		FloatVal opacity;		

		FloatVal position;
		FloatVal rotate;
		FloatVal scale;

		Transform();

		void Load(const Json::Value& val);
	};

	struct Layer
	{
		CU_STR name;
		CU_STR ref_id;

		int layer_id;
		int layer_type;

		int parent_id;

		CU_STR cl;	// class

		float in_frame, out_frame;

		int start_frame;

		int auto_ori;	// Auto-Orient along path AE property.
		int blend_mode;

		// comp
		int comp_width, comp_height;

		// solid type
		int solid_width, solid_height;
		CU_STR solid_color;

		Transform trans;

		Layer();

		bool Load(const Json::Value& val);
	};

public:
	const CU_VEC<Asset>& GetAssets() const { return m_assets; }
	const CU_VEC<Layer>& GetLayers() const { return m_layers; }

private:
	void Clear();

	void ParseHeader(const Json::Value& val);
	void ParseAssets(const Json::Value& val, const CU_STR& dir);
	static void ParseLayers(const Json::Value& val, CU_VEC<Layer>& layers);

private:
	CU_STR m_version;
	CU_STR m_name;

	int m_frame_rate;

	int m_width, m_height;

	int m_start_frame, m_end_frame;

	CU_VEC<Asset> m_assets;
	CU_VEC<Layer> m_layers;

}; // BodymovinParser

}

#endif // _S2LOADER_BODYMOVIN_PARSER_H_
