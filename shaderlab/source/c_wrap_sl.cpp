#include "c_wrap_sl.h"
#include "ShaderMgr.h"
#include "Shape2Shader.h"
#include "Shape3Shader.h"
#include "Sprite2Shader.h"
#include "Sprite3Shader.h"
#include "BlendShader.h"
#include "FilterShader.h"
#include "Model3Shader.h"
#include "MaskShader.h"
#include "SubjectMVP2.h"
#include "SubjectMVP3.h"
#include "HeatHazeProg.h"
#include "BurningMapProg.h"
#include "ColGradingProg.h"
#include "RenderShader.h"
#include "Statistics.h"

#include <unirender/RenderContext.h>
#include <sm_c_vector.h>
#include <sm_c_matrix.h>

namespace sl
{

/**
 *  @brief
 *    common
 */

extern "C"
void sl_create(void* render_context)
{
	ShaderMgr::Instance()->SetContext(static_cast<ur::RenderContext*>(render_context));
}

extern "C"
void sl_release()
{
    SubjectMVP2::Instance()->Clear();
    SubjectMVP3::Instance()->Clear();
}

extern "C"
void sl_create_shader(enum SHADER_TYPE type)
{
	ShaderMgr* mgr = ShaderMgr::Instance();
	ur::RenderContext* rc = mgr->GetContext();

	ShaderType sl_type = MAX_SHADER;
	Shader* shader = NULL;
	switch (type)
	{
	case ST_SHAPE2:
		sl_type = SHAPE2;
		shader = new Shape2Shader(rc);
		break;
	case ST_SHAPE3:
		sl_type = SHAPE3;
		shader = new Shape3Shader(rc);
		break;
	case ST_SPRITE2:
		sl_type = SPRITE2;
		shader = new Sprite2Shader(rc);
		break;
	case ST_SPRITE3:
		sl_type = SPRITE3;
		shader = new Sprite3Shader(rc);
		break;
	case ST_BLEND:
		sl_type = BLEND;
		shader = new BlendShader(rc);
		break;
	case ST_FILTER:
		sl_type = FILTER;
		shader = new FilterShader(rc);
		break;
	case ST_MODEL3:
		sl_type = MODEL3;
		shader = new Model3Shader(rc);
		break;
	case ST_MASK:
		sl_type = MASK;
		shader = new MaskShader(rc);
		break;
	}
	if (shader) {
		mgr->CreateShader(sl_type, shader);
	}
}

extern "C"
void sl_release_shader(enum SHADER_TYPE type) {
	ShaderMgr::Instance()->ReleaseShader((ShaderType)type);
}

extern "C"
void sl_set_shader(enum SHADER_TYPE type)
{
	if (type >= 0 && type < ST_MAX_SHADER) {
		ShaderMgr::Instance()->SetShader(ShaderType(type));
	}
}

extern "C"
int  sl_is_shader(enum SHADER_TYPE type) {
	if (type >= 0 && type < ST_MAX_SHADER) {
		return ShaderMgr::Instance()->GetShaderType() == type ? 1 : 0;
	} else {
		return 0;
	}	
}

extern "C"
void sl_on_projection2(int w, int h) {
	SubjectMVP2::Instance()->NotifyProjection(w, h);
}

extern "C"
void sl_on_projection3(const sm_mat4* mat) {
	SubjectMVP3::Instance()->NotifyProjection(sm::mat4(mat->x));
}

extern "C"
void sl_on_modelview2(float x, float y, float sx, float sy) {
	SubjectMVP2::Instance()->NotifyModelview(x, y, sx, sy);
}

extern "C"
void sl_on_modelview3(const sm_mat4* mat) {
	SubjectMVP3::Instance()->NotifyModelview(sm::mat4(mat->x));
}

extern "C"
void sl_set_texture(int id) {
	ShaderMgr::Instance()->GetContext()->BindTexture(id, 0);
}

extern "C"
int  sl_get_texture() {
	return ShaderMgr::Instance()->GetContext()->GetCurrTexture();
}

extern "C"
void sl_set_target(int id) {
	ShaderMgr::Instance()->GetContext()->BindRenderTarget(id);
}

extern "C"
void sl_set_blend(int m1, int m2) {
	ShaderMgr::Instance()->GetContext()->SetBlend(m1, m2);
}

extern "C"
void sl_set_default_blend() {
	ShaderMgr::Instance()->GetContext()->SetDefaultBlend();
}

extern "C"
void sl_set_blend_equation(int func) {
	ShaderMgr::Instance()->GetContext()->SetBlendEquation(func);
}

extern "C"
void sl_render_clear(unsigned long argb) {
	ShaderMgr::Instance()->GetContext()->Clear(argb);
}

extern "C"
int  sl_render_version() {
	return ShaderMgr::Instance()->GetContext()->RenderVersion();
}

extern "C"
void sl_enable_scissor(int enable) {
	ShaderMgr::Instance()->GetContext()->EnableScissor(enable);
}

extern "C"
void sl_set_viewport(int x, int y, int w, int h) {
	ShaderMgr::Instance()->GetContext()->SetViewport(x, y, w, h);
}

extern "C"
void sl_flush() 
{
	Shader* shader = ShaderMgr::Instance()->GetShader();
	if (shader) {
		shader->Commit();
	}
}

extern "C"
void* sl_get_render_context()
{
	return ShaderMgr::Instance()->GetContext();	
}

extern "C"
int sl_get_drawcall()
{
	return Statistics::Instance()->GetDrawCall();
}

extern "C"
int sl_get_vertices_count()
{
	return Statistics::Instance()->GetVertices();
}

extern "C"
void sl_reset_statistics()
{
	return Statistics::Instance()->Reset();
}

/**
 *  @brief
 *    shape2 shader
 */

extern "C"
void sl_shape2_color(uint32_t color) 
{
	ShaderMgr* mgr = ShaderMgr::Instance();
	if (Shape2Shader* shader = static_cast<Shape2Shader*>(mgr->GetShader(SHAPE2))) {
		shader->SetColor(color);
	}
}

extern "C"
void sl_shape2_type(int type)
{
	ShaderMgr* mgr = ShaderMgr::Instance();
	if (Shape2Shader* shader = static_cast<Shape2Shader*>(mgr->GetShader(SHAPE2))) {
		shader->SetType(type);
	}
}

extern "C"
void sl_shape2_draw(const float* positions, int count)
{
	ShaderMgr* mgr = ShaderMgr::Instance();
	if (Shape2Shader* shader = static_cast<Shape2Shader*>(mgr->GetShader(SHAPE2))) {
		shader->Draw(positions, count);
	}
}

extern "C"
void sl_shape2_draw_with_color(const float* positions, const uint32_t* colors, int count) {
	ShaderMgr* mgr = ShaderMgr::Instance();
	if (Shape2Shader* shader = static_cast<Shape2Shader*>(mgr->GetShader(SHAPE2))) {
		shader->Draw(positions, colors, count);
	}	
}

extern "C"
void sl_shape2_draw_node(float x, float y, int dummy)
{
	ShaderMgr* mgr = ShaderMgr::Instance();
	if (Shape2Shader* shader = static_cast<Shape2Shader*>(mgr->GetShader(SHAPE2))) {
		shader->Draw(x, y, dummy != 0);
	}
}

/**
 *  @brief
 *    shape3 shader
 */

extern "C"
void sl_shape3_color(uint32_t color) 
{
	ShaderMgr* mgr = ShaderMgr::Instance();
	if (Shape3Shader* shader = static_cast<Shape3Shader*>(mgr->GetShader(SHAPE3))) {
		shader->SetColor(color);
	}
}

extern "C"
void sl_shape3_type(int type)
{
	ShaderMgr* mgr = ShaderMgr::Instance();
	if (Shape3Shader* shader = static_cast<Shape3Shader*>(mgr->GetShader(SHAPE3))) {
		shader->SetType(type);
	}
}

extern "C"
void sl_shape3_draw(const float* positions, int count)
{
	ShaderMgr* mgr = ShaderMgr::Instance();
	if (Shape3Shader* shader = static_cast<Shape3Shader*>(mgr->GetShader(SHAPE3))) {
		shader->Draw(positions, count);
	}
}

extern "C"
void sl_shape3_draw_node(float x, float y, float z, int dummy)
{
	ShaderMgr* mgr = ShaderMgr::Instance();
	if (Shape3Shader* shader = static_cast<Shape3Shader*>(mgr->GetShader(SHAPE3))) {
		shader->Draw(x, y, z, dummy != 0);
	}
}

/**
 *  @brief
 *    sprite2 shader
 */

extern "C"
void sl_sprite2_set_color(uint32_t color, uint32_t additive)
{
	ShaderMgr* mgr = ShaderMgr::Instance();
	if (Sprite2Shader* shader = static_cast<Sprite2Shader*>(mgr->GetShader(SPRITE2))) {
		shader->SetColor(color, additive);
	}
}

extern "C"
void sl_sprite2_set_map_color(uint32_t rmap, uint32_t gmap, uint32_t bmap)
{
	ShaderMgr* mgr = ShaderMgr::Instance();
	if (Sprite2Shader* shader = static_cast<Sprite2Shader*>(mgr->GetShader(SPRITE2))) {
		shader->SetColorMap(rmap, gmap, bmap);
	}
}

extern "C"
void sl_sprite2_draw(const float* positions, const float* texcoords, int texid)
{
	ShaderMgr* mgr = ShaderMgr::Instance();
	if (Sprite2Shader* shader = static_cast<Sprite2Shader*>(mgr->GetShader(SPRITE2))) {
		shader->Draw(positions, texcoords, texid);
	}
}

/**
 *  @brief
 *    sprite3 shader
 */

extern "C"
void sl_sprite3_set_color(uint32_t color, uint32_t additive) {
	ShaderMgr* mgr = ShaderMgr::Instance();
	if (Sprite3Shader* shader = static_cast<Sprite3Shader*>(mgr->GetShader(SPRITE3))) {
		shader->SetColor(color, additive);
	}
}

extern "C"
void sl_sprite3_set_map_color(uint32_t rmap, uint32_t gmap, uint32_t bmap) {
	ShaderMgr* mgr = ShaderMgr::Instance();
	if (Sprite3Shader* shader = static_cast<Sprite3Shader*>(mgr->GetShader(SPRITE3))) {
		shader->SetColorMap(rmap, gmap, bmap);
	}
}

extern "C"
void sl_sprite3_draw(const float* positions, const float* texcoords, int texid) {
	ShaderMgr* mgr = ShaderMgr::Instance();
	if (Sprite3Shader* shader = static_cast<Sprite3Shader*>(mgr->GetShader(SPRITE3))) {
		shader->Draw(positions, texcoords, texid);
	}
}

/**
 *  @brief
 *    filter shader
 */

extern "C"
void sl_filter_set_mode(int mode) {
	ShaderMgr* mgr = ShaderMgr::Instance();
	if (FilterShader* shader = static_cast<FilterShader*>(mgr->GetShader(FILTER))) {
		shader->SetMode(FILTER_MODE(mode));
	}
}

extern "C"
void sl_filter_set_heat_haze_factor(float distortion, float rise) {
	ShaderMgr* mgr = ShaderMgr::Instance();
	FilterShader* shader = static_cast<FilterShader*>(mgr->GetShader(FILTER));
	if (shader) {
		HeatHazeProg* prog = static_cast<HeatHazeProg*>(shader->GetProgram(FM_HEAT_HAZE));
		if (prog) {
			prog->SetFactor(distortion, rise);
		}
	}
}

extern "C"
void sl_filter_set_heat_haze_texture(int id) {
	ShaderMgr* mgr = ShaderMgr::Instance();
	FilterShader* shader = static_cast<FilterShader*>(mgr->GetShader(FILTER));
	if (shader) {
		HeatHazeProg* prog = static_cast<HeatHazeProg*>(shader->GetProgram(FM_HEAT_HAZE));
		if (prog) {
			prog->SetDistortionMapTex(id);
		}
	}
}

extern "C"
void sl_filter_set_burning_map_upper_texture(int id) {
	ShaderMgr* mgr = ShaderMgr::Instance();
	FilterShader* shader = static_cast<FilterShader*>(mgr->GetShader(FILTER));
	if (shader) {
		BurningMapProg* prog = static_cast<BurningMapProg*>(shader->GetProgram(FM_BURNING_MAP));
		if (prog) {
			prog->SetUpperTex(id);
		}
	}
}

extern "C"
void sl_filter_set_burning_map_height_texture(int id) {
	ShaderMgr* mgr = ShaderMgr::Instance();
	FilterShader* shader = static_cast<FilterShader*>(mgr->GetShader(FILTER));
	if (shader) {
		BurningMapProg* prog = static_cast<BurningMapProg*>(shader->GetProgram(FM_BURNING_MAP));
		if (prog) {
			prog->SetHeightMapTex(id);
		}
	}
}

extern "C"
void sl_filter_set_burning_map_border_texture(int id) {
	ShaderMgr* mgr = ShaderMgr::Instance();
	FilterShader* shader = static_cast<FilterShader*>(mgr->GetShader(FILTER));
	if (shader) {
		BurningMapProg* prog = static_cast<BurningMapProg*>(shader->GetProgram(FM_BURNING_MAP));
		if (prog) {
			prog->SetBorderGradientTex(id);
		}
	}
}

extern "C"
void sl_filter_set_col_grading_texture(int id) {
	ShaderMgr* mgr = ShaderMgr::Instance();
	FilterShader* shader = static_cast<FilterShader*>(mgr->GetShader(FILTER));
	if (shader) {
		ColGradingProg* prog = static_cast<ColGradingProg*>(shader->GetProgram(FM_COL_GRADING));
		if (prog) {
			prog->SetLUTTex(id);
		}
	}
}

extern "C"
void sl_filter_update(float dt) {
	ShaderMgr* mgr = ShaderMgr::Instance();
	if (FilterShader* shader = static_cast<FilterShader*>(mgr->GetShader(FILTER))) {
		shader->UpdateTime(dt);
	}
}

extern "C"
void sl_filter_clear_time() {
	ShaderMgr* mgr = ShaderMgr::Instance();
	if (FilterShader* shader = static_cast<FilterShader*>(mgr->GetShader(FILTER))) {
		shader->ClearTime();
	}
}

extern "C"
void sl_filter_set_color(uint32_t color, uint32_t additive)
{
	ShaderMgr* mgr = ShaderMgr::Instance();
	if (FilterShader* shader = static_cast<FilterShader*>(mgr->GetShader(FILTER))) {
		shader->SetColor(color, additive);
	}
}

extern "C"
void sl_filter_draw(const float* positions, const float* texcoords, int texid) {
	ShaderMgr* mgr = ShaderMgr::Instance();
	if (FilterShader* shader = static_cast<FilterShader*>(mgr->GetShader(FILTER))) {
		shader->Draw(positions, texcoords, texid);
	}
}

/**
 *  @brief
 *    blend shader
 */

extern "C"
void sl_blend_set_mode(int mode) {
	ShaderMgr* mgr = ShaderMgr::Instance();	
	if (BlendShader* shader = static_cast<BlendShader*>(mgr->GetShader(BLEND))) {
		shader->SetMode(mode);
	}
}

extern "C"
void sl_blend_set_color(uint32_t color, uint32_t additive) {
	ShaderMgr* mgr = ShaderMgr::Instance();
	if (BlendShader* shader = static_cast<BlendShader*>(mgr->GetShader(BLEND))) {
		shader->SetColor(color, additive);
	}
}

extern "C"
void sl_blend_draw(const float* positions, const float* texcoords_blend, 
				   const float* texcoords_base, int tex_blend, int tex_base) {
	ShaderMgr* mgr = ShaderMgr::Instance();
	if (BlendShader* shader = static_cast<BlendShader*>(mgr->GetShader(BLEND))) {
		shader->Draw(positions, texcoords_blend, texcoords_base, tex_blend, tex_base);
	}
}

/**
 *  @brief
 *    mask shader
 */

extern "C"
void sl_mask_draw(const float* positions, const float* texcoords, 
				  const float* texcoords_mask, int tex, int tex_mask) {
	ShaderMgr* mgr = ShaderMgr::Instance();
	if (MaskShader* shader = static_cast<MaskShader*>(mgr->GetShader(MASK))) {
		shader->Draw(positions, texcoords, texcoords_mask, tex, tex_mask);
	}
}

}