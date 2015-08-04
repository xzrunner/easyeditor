#include "Symbol.h"
#include "ParticleSystem.h"
#include "FileIO.h"

namespace eparticle3d
{

Symbol::Symbol()
	: m_ps(NULL)
	, m_time(0)
{
}

Symbol::Symbol(const Symbol& s)
	: d2d::ISymbol(s)
	, m_time(s.m_time)
{
//	m_ps = new ParticleSystem(*s.m_ps);
}

Symbol::~Symbol()
{
	if (m_ps)
	{
		m_ps->Release();
		m_ps = NULL;
	}
}

Symbol* Symbol::Clone() const
{
	return new Symbol(*this);
}

void Symbol::ReloadTexture() const
{
	m_ps->ReloadTexture();
}

void Symbol::Draw(const d2d::Matrix& mt,
				  const d2d::Colorf& mul, 
				  const d2d::Colorf& add,
				  const d2d::Colorf& r_trans,
				  const d2d::Colorf& g_trans,
				  const d2d::Colorf& b_trans,
				  const d2d::ISprite* sprite) const
{
	if (!m_ps) {
		return;
	}

	clock_t curr = clock();
	if (m_time == 0) {
		m_time = curr;
		return;
	}

	d2d::ShaderMgr* shader = d2d::ShaderMgr::Instance();
	shader->SetSpriteColor(mul, add);
	shader->SetSpriteColorTrans(r_trans, g_trans, b_trans);

	float dt = (float)(curr - m_time) / CLOCKS_PER_SEC;
	bool loop = d2d::Config::Instance()->GetSettings().particle3d_loop;
	m_ps->SetLoop(loop);
	m_ps->update(dt);
	m_ps->draw(mt);

	m_time = curr;
}

d2d::Rect Symbol::GetSize(const d2d::ISprite* sprite) const
{
//	return m_region;
	return d2d::Rect(200, 200);
}

void Symbol::Start() const
{
	m_ps->start();
}

void Symbol::LoadResources()
{
	m_ps = FileIO::LoadPS(m_filepath.c_str());	
}

}