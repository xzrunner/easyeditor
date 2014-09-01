#include "Symbol.h"
#include "ParticleSystem.h"
#include "FileAdapter.h"

namespace eparticle3d
{

Symbol::Symbol()
	: m_ps(NULL)
{
}

Symbol::Symbol(const Symbol& s)
	: d2d::ISymbol(s)
{
//	m_ps = new ParticleSystem(*s.m_ps);
}

Symbol::~Symbol()
{
	if (m_ps)
	{
		m_ps->release();
		m_ps = NULL;
	}
}

Symbol* Symbol::clone() const
{
	return new Symbol(*this);
}

void Symbol::reloadTexture() const
{
	m_ps->reloadTexture();
}

void Symbol::draw(const d2d::Screen& scr,
				  const d2d::Matrix& mt,
				  const d2d::Colorf& mul, 
				  const d2d::Colorf& add,
				  const d2d::ISprite* sprite) const
{
	if (!m_ps) {
		return;
	}

	static clock_t time = 0;
	clock_t curr = clock();
	if (time != 0) {
		if (m_ps->IsEmpty()) {
			m_ps->origin = sprite->getPosition();
			m_ps->start();
			m_ps->reset();		
		}

		m_ps->update((float)(curr - time) / CLOCKS_PER_SEC);
		m_ps->draw(scr, mt);
	}
	time = curr;
}

d2d::Rect Symbol::getSize(const d2d::ISprite* sprite) const
{
//	return m_region;
	return d2d::Rect(200, 200);
}

void Symbol::refresh()
{
}

void Symbol::loadResources()
{
	FileAdapter adapter;
	adapter.load(m_filepath);

	if (!m_ps) {
		m_ps = new ParticleSystem(1000);
	}

	m_ps->setCount(adapter.count);
	m_ps->setEmissionTime(adapter.emission_time);
	m_ps->setLife(adapter.min_life, adapter.max_life);
	m_ps->setHori(adapter.min_hori, adapter.max_hori);
	m_ps->setVert(adapter.min_vert, adapter.max_vert);
	m_ps->setSpeed(adapter.min_spd, adapter.max_spd);
	m_ps->setGravity(adapter.gravity);
	m_ps->setFadeoutTime(adapter.fadeout_time);
	m_ps->setBounce(adapter.bounce);
	m_ps->setStartRadius(adapter.start_radius);
	for (size_t i = 0, n = adapter.children.size(); i < n; ++i)
	{
		const FileAdapter::Child& child = adapter.children[i];

		ParticleChild* pc = new ParticleChild;

		// todo release symbol
		pc->symbol = d2d::SymbolMgr::Instance()->fetchSymbol(child.filepath);

		pc->start_scale = child.start_scale * 0.01f;
		pc->end_scale = child.end_scale * 0.01f;

		pc->min_rotate = child.min_rotate * d2d::TRANS_DEG_TO_RAD;
		pc->max_rotate = child.max_rotate * d2d::TRANS_DEG_TO_RAD;

		m_ps->addChild(pc);
	}

	
}

}