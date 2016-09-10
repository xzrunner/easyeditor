#include "GUM_DTex.h"
#include "RenderContext.h"

#include <dtex.h>
#include <shaderlab.h>
#include <render/render.h>

#include <sprite2/S2_RVG.h>
#include <sprite2/RenderScissor.h>
#include <sprite2/RenderCtxStack.h>
#include <sprite2/Texture.h>

#include <assert.h>

namespace gum
{

SINGLETON_DEFINITION(DTex)

static void (*DRAW_BEGIN)() = NULL;
static void (*DRAW_END)() = NULL;

//////////////////////////////////////////////////////////////////////////

static void _program(int n) 
{
	switch (n) 
	{
	case DTEX_PROGRAM_NULL:
//		ShaderMgr::Instance()->SetShader(ShaderMgr::COUNT);
		break;
	case DTEX_PROGRAM_NORMAL:
		sl::ShaderMgr::Instance()->SetShader(sl::SPRITE2);
		break;
	default:
		assert(0);
	}
}

static void _blend(int mode)
{
	assert(mode == 0);
//	ShaderMgr::Instance()->SetBlendMode(0);
}

static void _set_texture(int id)
{
	sl::ShaderMgr::Instance()->GetContext()->SetTexture(id, 0);
}

static int _get_texture()
{
	return 	sl::ShaderMgr::Instance()->GetContext()->GetTexture();
}

static void _set_target(int id)
{
	sl::ShaderMgr::Instance()->GetContext()->SetTarget(id);
}

static int _get_target()
{
	return sl::ShaderMgr::Instance()->GetContext()->GetTarget();
}

static void _draw_begin() 
{
	if (DRAW_BEGIN) {
		DRAW_BEGIN();
	} else {
		s2::RenderCtxStack::Instance()->Push(s2::RenderCtx(2, 2));
	}

	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl_mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* sl_shader = static_cast<sl::Sprite2Shader*>(sl_mgr->GetShader());
	sl_shader->SetColor(0xffffffff, 0);
	sl_shader->SetColorMap(0x000000ff, 0x0000ff00, 0x00ff0000);
}

static void _draw(const float vb[16], int texid)
{
	sm::vec2 vertices[4], texcoords[4];
	for (int i = 0; i < 4; ++i) {
		vertices[i].x  = vb[i * 4];
		vertices[i].y  = vb[i * 4 + 1];
		texcoords[i].x = vb[i * 4 + 2];
		texcoords[i].y = vb[i * 4 + 3];
	}

	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* sl_shader = static_cast<sl::Sprite2Shader*>(sl_mgr->GetShader());
	sl_shader->Draw(&vertices[0].x, &texcoords[0].x, texid);
}

static void _draw_end()
{
	sl::ShaderMgr::Instance()->GetShader()->Commit();

	if (DRAW_END) {
		DRAW_END();
	} else {
		s2::RenderCtxStack::Instance()->Pop();
	}
}

static void _draw_flush()
{
	sl::Shader* shader = sl::ShaderMgr::Instance()->GetShader();
	if (shader) {
		shader->Commit();
	}
}

static void _scissor_enable() 
{
	s2::RenderScissor::Instance()->Open();
}

static void _scissor_disable() 
{
	s2::RenderScissor::Instance()->Close();
}

#define IS_POT(x) ((x) > 0 && ((x) & ((x) -1)) == 0)

static int _texture_create(int type, int width, int height, const void* data, int channel,unsigned int id) {
	TEXTURE_FORMAT format = TEXTURE_RGBA8;
	switch (type) {
	case DTEX_TF_RGBA8:
		format = TEXTURE_RGBA8;
		break;
	case DTEX_TF_PVR4:
		format = TEXTURE_PVR4;
		break;
	default:
		assert(0);
	}

	return RenderContext::Instance()->CreateTexture(static_cast<const uint8_t*>(data), width, height, format);
}

static void 
_texture_release(int id) {
	RenderContext::Instance()->ReleaseTexture(id);
}

static void
_texture_update(const void* pixels, int x, int y, int w, int h, unsigned int id) {
	RenderContext::Instance()->UpdateTexture(static_cast<const uint8_t*>(pixels), x, y, w, h, id);
}

static int
_texture_id(int id) {
	return id;
}

static void 
_clear_color(float xmin, float ymin, float xmax, float ymax) {
// 	glDisable(GL_BLEND);
// 	//	glBlendFunc(GL_ONE, GL_ZERO);
// 
// 	ShaderMgr* mgr = ShaderMgr::Instance();
// 
// 	mgr->SetShader(ShaderMgr::SHAPE);
// 	ShapeShader* shader = static_cast<ShapeShader*>(mgr->GetShader(ShaderMgr::SHAPE));
// 	
// 	shader->SetColor(s2::Color(0, 0, 0, 0));
// 
// 	std::vector<sm::vec2> triangles;
// 	triangles.push_back(sm::vec2(xmin, ymin));
// 	triangles.push_back(sm::vec2(xmin, ymax));
// 	triangles.push_back(sm::vec2(xmax, ymin));
// 	triangles.push_back(sm::vec2(xmax, ymax));
// 
// 	s2::RVG::TriangleStrip(triangles);
// 
// 	mgr->Commit();
// 	ShaderLab::Instance()->Flush();
// 
// 	//	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
// 	glEnable(GL_BLEND);

	//////////////////////////////////////////////////////////////////////////

	dtex_gl_clear_color(0, 0, 0, 0);
}

//////////////////////////////////////////////////////////////////////////

static const char* CFG =
	"{ \n"
	"	\"open_c1\" : true,		\n"
	"	\"open_c2\" : true,		\n"
	"	\"open_c3\" : false,	\n"
	"	\"open_c4\" : false,	\n"
	"	\"open_cg\" : true,		\n"
	"	\"open_cs\" : true,		\n"
	"	\"c1_tex_size\" : 1024, \n"
	"	\"c2_tex_size\" : 2048	\n"
	"} \n"
	;

DTex::DTex()
{
	dtex_shader_init(&_program, &_blend, &_set_texture, &_get_texture, &_set_target, &_get_target,
		&_draw_begin, &_draw, &_draw_end, &_draw_flush, &_scissor_enable, _scissor_disable);

	dtex_gl_init(&_clear_color);
	dtex_gl_texture_init(&_texture_create, &_texture_release, &_texture_update, &_texture_id);

	dtexf_create(CFG);
}

void DTex::InitHook(void (*draw_begin)(), void (*draw_end)())
{
	DRAW_BEGIN = draw_begin;
	DRAW_END = draw_end;
}

void DTex::LoadBegin()
{
	dtexf_c2_load_begin();
}

int DTex::Load(const std::string& filepath, const s2::Texture* tex)
{
	std::map<std::string, int>::iterator itr = m_map_filepath.find(filepath);
	if (itr != m_map_filepath.end()) {
		return itr->second;
	}

	int key = m_map_filepath.size();
	m_map_filepath.insert(std::make_pair(filepath, key));
	sm::vec2 sz = tex->GetSize();
	dtexf_c2_load_tex(tex->GetTexID(), (int)sz.x, (int)sz.y, key);

	return key;
}

void DTex::LoadEnd()
{
	dtexf_c2_load_end();
}

float* DTex::Query(const std::string& filepath, const s2::Texture* tex, int* id)
{
	if (filepath.empty()) {
		return NULL;
	}

	int key;
	std::map<std::string, int>::iterator itr = m_map_filepath.find(filepath);
	if (itr != m_map_filepath.end()) {
		key = itr->second;
	} else {
		dtexf_c2_load_begin();
		key = Load(filepath, tex);
		dtexf_c2_load_end();
	}
	return dtexf_c2_query_tex(key, id);
}

void DTex::Clear()
{
	dtexf_c2_clear();
}

dtex_cg* DTex::GetDtexCG()
{
	return dtexf_get_cg();
}

void DTex::OnSize(int w, int h)
{
	dtex_set_screen(static_cast<float>(w), static_cast<float>(h), 1);
}

void DTex::DebugDraw() const
{
	if (sl::Shader* shader = sl::ShaderMgr::Instance()->GetShader()) {
		shader->Commit();
	}
	dtexf_debug_draw();
}

}