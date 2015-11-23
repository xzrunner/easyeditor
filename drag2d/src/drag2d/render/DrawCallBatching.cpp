#include "DrawCallBatching.h"

#include "dataset/Image.h"
#include "render/ShaderMgr.h"

#include <dtex_facade.h>

namespace d2d
{

DrawCallBatching* DrawCallBatching::m_instance = NULL;

//////////////////////////////////////////////////////////////////////////

static int TEX_ID = 0;

void program(int n) 
{
	assert(n == 1);
	ShaderMgr::Instance()->sprite();
}

void blend(int mode)
{
	assert(mode == 0);
//	ShaderMgr::Instance()->SetBlendMode(0);
}

void set_texture(int id)
{
	TEX_ID = id;
	ShaderMgr::Instance()->SetTexture(id);
}

int get_texture()
{
	return ShaderMgr::Instance()->GetTexID();
}

void set_target(int id)
{
	ShaderMgr::Instance()->SetFBO(id);
}

int get_target()
{
	return ShaderMgr::Instance()->GetFboID();
}

void draw(const float vb[16])
{
	ShaderMgr::Instance()->Draw(vb, TEX_ID);
}

void flush()
{
	ShaderMgr::Instance()->Flush();
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
	dtex_shader_init(&program, &blend, &set_texture, &get_texture, &set_target, &get_target,
		&draw, &flush);

	dtexf_create(CFG);

// 	dtex_shader_load();
// 	dtex_render_init();
// 
// 	m_loader = dtexloader_create();
// 	m_c2 = dtex_c2_create(4096, true, 1, false);
}

DrawCallBatching::~DrawCallBatching()
{
// 	dtexloader_release(m_loader);
// 	dtex_c2_release(m_c2);
}

void DrawCallBatching::Begin()
{
	dtexf_c2_load_begin();
}

int DrawCallBatching::Add(const Image* img)
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

void DrawCallBatching::End()
{
	dtexf_c2_load_end();
}

float* DrawCallBatching::Query(const Image* img, int* id)
{
	int key;
	std::map<std::string, int>::iterator itr = m_path2id.find(img->GetFilepath());
	if (itr != m_path2id.end()) {
		key = itr->second;
	} else {
		key = Add(img);
	}
	return dtexf_c2_query_tex(key, id);
}

DrawCallBatching* DrawCallBatching::Instance()
{
	if (!m_instance) {
		m_instance = new DrawCallBatching();
	}
	return m_instance;
}

}