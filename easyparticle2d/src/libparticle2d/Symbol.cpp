#include "Symbol.h"
#include "ParticleSystem.h"
#include "FileIO.h"

#include <time.h>

namespace eparticle2d
{

Symbol::Symbol()
	: m_ps(NULL)
	, m_curr_frame(0)
{
}

Symbol::Symbol(const Symbol& s)
	: d2d::ISymbol(s)
{
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

	DrawPS(mt);

	if (sprite) {
		DrawBackground(sprite->GetPosition());
	}
}

d2d::Rect Symbol::GetSize(const d2d::ISprite* sprite) const
{
	return d2d::Rect(200, 200);
}

void Symbol::ResetPS()
{
	m_ps->Reset();
	m_ps->Start();
}

void Symbol::SetPS(ParticleSystem* ps)
{
	d2d::obj_assign<ParticleSystem>(m_ps, ps);
}

void Symbol::LoadResources()
{
	m_ps = FileIO::LoadPS(m_filepath.c_str());	
}

void Symbol::DrawPS(const d2d::Matrix& mt) const
{
	static clock_t last_time = 0;
	if (last_time == 0) {
		last_time = clock();
	} else {
		clock_t curr_time = clock();
		float dt = (float)(curr_time - last_time) / CLOCKS_PER_SEC;
		m_ps->Update(dt);
		last_time = curr_time;
	}
	m_ps->Draw(mt);
}

void Symbol::DrawBackground(const d2d::Vector& pos) const
{
	d2d::Matrix mat;
	mat.translate(pos.x, pos.y);

	d2d::Rect r = GetSize();

	d2d::PrimitiveDraw::rect(mat, r, d2d::LIGHT_GREY_LINE);
}

}