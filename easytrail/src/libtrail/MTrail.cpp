#include "MTrail.h"

#include <ee/Symbol.h>
#include <ee/SpriteRenderer.h>

#include <mt_2d.h>

#include <time.h>
#include <assert.h>

namespace etrail
{

MTrail* MTrail::m_instance = NULL;

MTrail* MTrail::Instance()
{
	if (!m_instance) {
		m_instance = new MTrail();
	}
	return m_instance;
}

MTrail::MTrail()
	: m_time(0)
{
	Init();

	ee::SceneNodeMgr::Instance()->Add(this);
}

bool MTrail::Update(float dt)
{
	return true;
}

void MTrail::Draw() const
{
	
}

void MTrail::UpdateTime()
{
	static clock_t last = 0;
	if (last == 0) {
		last = clock();
	} else {
		clock_t curr = clock();
		if (curr != last) {
			float dt = (float)(curr - last) / CLOCKS_PER_SEC;
			m_time += dt;
			last = curr;
		}
	}
}

static void 
render_func(void* symbol, float* mat, float x, float y, const void* ud)
{
	assert(ud);
	const RenderParams* rp = (static_cast<const RenderParams*>(ud));
	assert(rp->trail);

	ee::Symbol* sym = static_cast<ee::Symbol*>(symbol);

	s2::RenderParams params;

	sm::mat4 _mat;
	_mat.x[0] = mat[0];
	_mat.x[1] = mat[1];
	_mat.x[4] = mat[2];
	_mat.x[5] = mat[3];
	_mat.x[12]= mat[4];
	_mat.x[13]= mat[5];
	params.mt = _mat * rp->mat;

	ee::SpriteRenderer::Draw(sym, params, sm::vec2(x, y));
}

void MTrail::Init()
{
	t2d_init();
	t2d_regist_cb(render_func);	
}

}