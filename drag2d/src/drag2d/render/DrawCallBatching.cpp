#include "DrawCallBatching.h"

#include "dataset/Image.h"
#include "render/ShaderMgr.h"

#include <dtex.h>

#include <gl/glew.h>

namespace d2d
{

DrawCallBatching* DrawCallBatching::m_instance = NULL;

//////////////////////////////////////////////////////////////////////////

static int TEX_ID = 0;

static void _program(int n) 
{
	switch (n) 
	{
	case DTEX_PROGRAM_NULL:
		ShaderMgr::Instance()->null();
		break;
	case DTEX_PROGRAM_NORMAL:
		ShaderMgr::Instance()->sprite();
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
	TEX_ID = id;
	ShaderMgr::Instance()->SetTexture(id);
}

static int _get_texture()
{
	return ShaderMgr::Instance()->GetTexID();
}

static void _set_target(int id)
{
	ShaderMgr::Instance()->SetFBO(id);
}

static int _get_target()
{
	return ShaderMgr::Instance()->GetFboID();
}

static int ori_width, ori_height;
static Vector ori_offset;
static float ori_scale;

static void _draw_begin() 
{
	ShaderMgr* shader = ShaderMgr::Instance();

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	ori_width = viewport[2];
	ori_height = viewport[3];

	shader->GetModelView(ori_offset, ori_scale);

	shader->SetModelView(Vector(0, 0), 1);
	shader->SetProjection(2, 2);
// 	glViewport(0, 0, 2, 2);
}

static void _draw(const float vb[16])
{
	ShaderMgr* shader = ShaderMgr::Instance();
	shader->Draw(vb, TEX_ID);
}

static void _draw_end()
{
	ShaderMgr* shader = ShaderMgr::Instance();

	shader->Flush();

	shader->SetModelView(ori_offset, ori_scale);
	shader->SetProjection(ori_width, ori_height);
// 	glViewport(0, 0, ori_width, ori_height);
}

#define IS_POT(x) ((x) > 0 && ((x) & ((x) -1)) == 0)

static int _texture_create(int type, int width, int height, const void* data, int channel,unsigned int id) {
	assert(type == DTEX_TF_RGBA8);

	// todo
	if ((type == DTEX_TF_RGBA8) || (IS_POT(width) && IS_POT(height))) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	} else {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}

	GLint _format = GL_RGBA;
	GLenum _type = GL_UNSIGNED_BYTE;

	bool is_compressed = false;
	unsigned int size = 0;
	uint8_t* uncompressed = NULL;

	switch (type) {
	case DTEX_TF_RGBA8:
		is_compressed = false;
		_format = GL_RGBA;
		_type = GL_UNSIGNED_BYTE;
		break;
	case DTEX_TF_RGBA4:
		is_compressed = false;
		_format = GL_RGBA;
		_type = GL_UNSIGNED_SHORT_4_4_4_4;
		break;
	case DTEX_TF_PVR2:
#ifdef __APPLE__
		is_compressed = true;
		_type = COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
		size = width * height * 8 * _type / 16;
#endif // __APPLE__
		break;
	case DTEX_TF_PVR4:
#ifdef __APPLE__
		is_compressed = true;
		_type = COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
		size = width * height * 8 * _type / 16;
#else
// 		is_compressed = false;
// 		_format = GL_RGBA;
// 		_type = GL_UNSIGNED_BYTE;
// 		uncompressed = dtex_pvr_decode(data, width, height);
#endif // __APPLE__
		break;
	case DTEX_TF_ETC1:
#ifdef __ANDROID__
		is_compressed = true;
		_type = GL_ETC1_RGB8_OES;
		size = width * height * 4 / 8;
#else
		is_compressed = false;
		_format = GL_RGBA;
		_type = GL_UNSIGNED_BYTE;
#ifdef USED_IN_EDITOR
//		uncompressed = dtex_etc1_decode(data, width, height);
#endif // USED_IN_EDITOR
#endif // __ANDROID__
		break;
	default:
		dtex_fault("dtex_gl_create_texture: unknown texture type.");
	}

	glActiveTexture(GL_TEXTURE0 + channel);
	if (id == 0) {
		glGenTextures(1, (GLuint*)&id);	
	}
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (is_compressed) {
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, _type, width, height, 0, size, data);	
	} else {
		if (uncompressed) {
			glTexImage2D(GL_TEXTURE_2D, 0, _format, (GLsizei)width, (GLsizei)height, 0, _format, _type, uncompressed);
			free(uncompressed);
		} else {
			glTexImage2D(GL_TEXTURE_2D, 0, _format, (GLsizei)width, (GLsizei)height, 0, _format, _type, data);
		}
	}

	return id;
}

static void 
_texture_release(int id) { 
	GLuint texid = (GLuint)id;
 	glActiveTexture(GL_TEXTURE0);
 	glDeleteTextures(1, &texid);
}

static void
_texture_update(const void* pixels, int x, int y, int w, int h, unsigned int id) {
	int old_id = ShaderMgr::Instance()->GetTexID();
 	glBindTexture(GL_TEXTURE_2D, id);
 	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
 	glBindTexture(GL_TEXTURE_2D, old_id);
}

static int
_texture_id(int id) {
	return id;
}

//////////////////////////////////////////////////////////////////////////

static const char* CFG =
	"{ \n"
	"	\"open_c1\" : false, \n"
	"	\"open_c2\" : true, \n"
	"	\"open_c3\" : false,	 \n"
	"	\"open_cg\" : true,	 \n"
	"	\"c2_tex_size\" : 2048	 \n"
	"} \n"
	;

DrawCallBatching::DrawCallBatching()
{
	dtex_shader_init(&_program, &_blend, &_set_texture, &_get_texture, &_set_target, &_get_target,
		&_draw_begin, &_draw, &_draw_end);

	dtex_gl_texture_init(&_texture_create, &_texture_release, &_texture_update, &_texture_id);

	dtexf_create(CFG);
}

DrawCallBatching::~DrawCallBatching()
{
}

void DrawCallBatching::LoadBegin()
{
	dtexf_c2_load_begin();
}

int DrawCallBatching::Load(const Image* img)
{
	const std::string& filepath = img->GetFilepath();
	std::map<std::string, int>::iterator itr = m_path2id.find(filepath);
	if (itr != m_path2id.end()) {
		return itr->second;
	}

	int key = m_path2id.size();
	m_path2id.insert(std::make_pair(filepath, key));
	dtexf_c2_load_tex(img->GetTexID(), img->GetOriginWidth(), img->GetOriginHeight(), key);

	return key;
}

void DrawCallBatching::LoadEnd()
{
	dtexf_c2_load_end();
}

void DrawCallBatching::ReloadBegin()
{
	dtexf_c2_reload_begin();
}

void DrawCallBatching::Reload(const Image* img)
{
	int key;
	std::map<std::string, int>::iterator itr = m_path2id.find(img->GetFilepath());
	assert(itr != m_path2id.end());
	key = itr->second;
	
	dtexf_c2_reload_tex(img->GetTexID(), img->GetOriginWidth(), img->GetOriginHeight(), key);
}

void DrawCallBatching::ReloadEnd()
{
	dtexf_c2_reload_end();
}

float* DrawCallBatching::Query(const Image* img, int* id)
{
	int key;
	std::map<std::string, int>::iterator itr = m_path2id.find(img->GetFilepath());
	if (itr != m_path2id.end()) {
		key = itr->second;
	} else {
		key = Load(img);
	}
	return dtexf_c2_query_tex(key, id);
}

dtex_cg* DrawCallBatching::GetDtexCG()
{
	return dtexf_get_cg();
}

DrawCallBatching* DrawCallBatching::Instance()
{
	if (!m_instance) {
		m_instance = new DrawCallBatching();
	}
	return m_instance;
}

}