#ifndef _S2LOADER_SPRITE_IO_H_
#define _S2LOADER_SPRITE_IO_H_

#include <SM_Vector.h>
#include <sprite2/typedef.h>
#include <sprite2/config.h>
#include <cu/cu_stl.h>
#include <s2s/NodeSprCommon.h>
#include <painting2/RenderColorCommon.h>
#include <painting2/RenderColorMap.h>
#include <painting2/CameraMode.h>
#include <painting2/FastBlendMode.h>
#include <painting2/BlendMode.h>
#include <painting2/RenderFilter.h>

#include <json/json.h>

namespace s2 { class Sprite; }
namespace pt2 { class RenderCamera; class RenderShader; }

namespace s2loader
{

class SpriteIO
{
public:
	SpriteIO(bool compress, bool render_open);

	void Load(const Json::Value& val, const s2::SprPtr& spr, const CU_STR& dir);
	void Store(Json::Value& val, const s2::SprConstPtr& spr, const CU_STR& dir);

	void Load(const Json::Value& val, const CU_STR& dir);
	void Store(Json::Value& val, const CU_STR& dir);

	void Load(const s2s::NodeSprCommon& node_spr, const s2::SprPtr& spr);
	void Load(const s2s::NodeSprCommon& node_spr);

protected:
	// geometry
	virtual void LoadGeometry(const s2::SprPtr& spr);
	virtual void StoreGeometry(const s2::SprConstPtr& spr);
	virtual void LoadGeometry(const Json::Value& val);
	virtual void StoreGeometry(Json::Value& val);

	// render
	virtual void LoadRender(const s2::SprPtr& spr);
	virtual void StoreRender(const s2::SprConstPtr& spr);
	virtual void LoadRender(const Json::Value& val, const CU_STR& dir);
	virtual void StoreRender(Json::Value& val, const CU_STR& dir);

	// info
	virtual void LoadInfo(const s2::SprPtr& spr);
	virtual void StoreInfo(const s2::SprConstPtr& spr);
	virtual void LoadInfo(const Json::Value& val);
	virtual void StoreInfo(Json::Value& val);

	// edit
	virtual void LoadEdit(const s2::SprPtr& spr);
	virtual void StoreEdit(const s2::SprConstPtr& spr);
	virtual void LoadEdit(const Json::Value& val);
	virtual void StoreEdit(Json::Value& val);

private:
	void LoadColor(const s2::SprPtr& spr);
	void StoreColor(const pt2::RenderColorCommon& col_common, const pt2::RenderColorMap& col_map);
	void LoadColor(const Json::Value& val);
	void StoreColor(Json::Value& val);

	void LoadShader(const s2::SprPtr& spr);
	void StoreShader(const pt2::RenderShader& shader);
	void LoadShader(const Json::Value& val, const CU_STR& dir);
	void StoreShader(Json::Value& val, const CU_STR& dir);

	void LoadCamera(const s2::SprPtr& spr);
	void StoreCamera(const pt2::RenderCamera& camera);	
	void LoadCamera(const Json::Value& val);
	void StoreCamera(Json::Value& val);

public:
	// geometry
	sm::vec2			   m_position;
	float				   m_angle;
	sm::vec2			   m_scale;
	sm::vec2			   m_shear;
	sm::vec2			   m_offset;

	// shader
	pt2::RenderColorCommon m_col_common;
	pt2::RenderColorMap    m_col_map;
	pt2::BlendMode		   m_blend;
	pt2::FastBlendMode	   m_fast_blend;
#ifdef S2_FILTER_FULL
	pt2::RenderFilterPtr   m_filter;
#else
	pt2::FilterMode        m_filter;
#endif // S2_FILTER_FULL
	float                  m_downsample;
	pt2::CameraMode		   m_camera;
	
	// info
	CU_STR			       m_name;
	bool                   m_need_actor;
	bool                   m_integrate;

	// edit
	bool			       m_visible;
	bool				   m_editable;

protected:
	bool m_compress;
	bool m_render_open;

}; // SpriteDataIO

}

#endif // _S2LOADER_SPRITE_IO_H_
