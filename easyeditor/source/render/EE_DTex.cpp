#include "EE_DTex.h"
#include "Image.h"
#include "RenderContextStack.h"
#include "EE_ShaderLab.h"
#include "EE_RVG.h"

#include <dtex.h>
#include <shaderlab.h>
#include <render/render.h>

#include <gl/glew.h>

#include <assert.h>

namespace ee
{

DTex* DTex::m_instance = NULL;

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

static sm::vec2 LAST_OFFSET;
static float LAST_SCALE;
static int LAST_WIDTH, LAST_HEIGHT;

static void _draw_begin() 
{
	RenderContextStack* ctx_stack = RenderContextStack::Instance();

	ctx_stack->GetModelView(LAST_OFFSET, LAST_SCALE);
	ctx_stack->GetProjection(LAST_WIDTH, LAST_HEIGHT);

	ctx_stack->SetModelView(sm::vec2(0, 0), 1);
	ctx_stack->SetProjection(2, 2);

	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl_mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* sl_shader = static_cast<sl::Sprite2Shader*>(sl_mgr->GetShader());
	sl_shader->SetColor(0xffffffff, 0);
	sl_shader->SetColorMap(0x000000ff, 0x0000ff00, 0x00ff0000);

// 	glViewport(0, 0, 2, 2);
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

	RenderContextStack* ctx_stack = RenderContextStack::Instance();
	ctx_stack->SetModelView(LAST_OFFSET, LAST_SCALE);
	ctx_stack->SetProjection(LAST_WIDTH, LAST_HEIGHT);

// 	glViewport(0, 0, ORI_WIDTH, ORI_HEIGHT);
}

static void _draw_flush()
{
	sl::Shader* shader = sl::ShaderMgr::Instance()->GetShader();
	if (shader) {
		shader->Commit();
	}
}

#define IS_POT(x) ((x) > 0 && ((x) & ((x) -1)) == 0)

static int _texture_create(int type, int width, int height, const void* data, int channel,unsigned int id) {
	EE_TEXTURE_FORMAT t = EE_TEXTURE_RGBA8;
	switch (type) {
	case DTEX_TF_RGBA8:
		t = EE_TEXTURE_RGBA8;
		break;
	case DTEX_TF_PVR4:
		t = EE_TEXTURE_PVR4;
		break;
	default:
		assert(0);
	}

	return ShaderLab::Instance()->CreateTexture(static_cast<const uint8_t*>(data), width, height, t);
}

static void 
_texture_release(int id) {
	ShaderLab::Instance()->ReleaseTexture(id);
}

static void
_texture_update(const void* pixels, int x, int y, int w, int h, unsigned int id) {
	ShaderLab::Instance()->UpdateTexture(static_cast<const uint8_t*>(pixels), x, y, w, h, id);
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
// 	RVG::TriangleStrip(triangles);
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
		&_draw_begin, &_draw, &_draw_end, &_draw_flush);

	dtex_gl_init(&_clear_color);
	dtex_gl_texture_init(&_texture_create, &_texture_release, &_texture_update, &_texture_id);

	dtexf_create(CFG);
}

DTex::~DTex()
{
}

void DTex::LoadBegin()
{
	dtexf_c2_load_begin();
}

int DTex::Load(const Image* img)
{
	const std::string& filepath = img->GetFilepath();
	std::map<std::string, int>::iterator itr = m_path2id.find(filepath);
	if (itr != m_path2id.end()) {
		return itr->second;
	}

	int key = m_path2id.size();
	m_path2id.insert(std::make_pair(filepath, key));
	dtexf_c2_load_tex(img->GetTexID(), img->GetClippedWidth(), img->GetClippedHeight(), key);

	return key;
}

void DTex::LoadEnd()
{
	dtexf_c2_load_end();
}

void DTex::ReloadBegin()
{
	dtexf_c2_reload_begin();
}

void DTex::Reload(const Image* img)
{
	int key;
	std::map<std::string, int>::iterator itr = m_path2id.find(img->GetFilepath());
	assert(itr != m_path2id.end());
	key = itr->second;
	
	dtexf_c2_reload_tex(img->GetTexID(), img->GetOriginWidth(), img->GetOriginHeight(), key);
}

void DTex::ReloadEnd()
{
	dtexf_c2_reload_end();
}

float* DTex::Query(const Image* img, int* id)
{
	std::string filepath = img->GetFilepath();
	if (filepath.empty()) {
		return NULL;
	}

	int key;
	std::map<std::string, int>::iterator itr = m_path2id.find(filepath);
	if (itr != m_path2id.end()) {
		key = itr->second;
	} else {
		dtexf_c2_load_begin();
		key = Load(img);
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
	sl::ShaderMgr::Instance()->GetShader()->Commit();
	dtexf_debug_draw();
}

DTex* DTex::Instance()
{
	if (!m_instance) {
		m_instance = new DTex();
	}
	return m_instance;
}

}