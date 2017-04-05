#include "CurrSprTreePath.h"

#include <sprite2/S2_Sprite.h>

#include <assert.h>

namespace ee
{

SINGLETON_DEFINITION(CurrSprTreePath);

CurrSprTreePath::CurrSprTreePath()
{
	m_spr_path.push(NULL);
	m_actor_path.push(NULL);
}

CurrSprTreePath::~CurrSprTreePath()
{
	while (!m_spr_path.empty())
	{
		const s2::Sprite* spr = m_spr_path.top();
		if (spr) {
			spr->RemoveReference();
		}
		m_spr_path.pop();
	}
}

void CurrSprTreePath::Push(const s2::Sprite* spr)
{
	const s2::Actor* actor = NULL;
	if (!m_actor_path.empty()) {
		actor = spr->QueryActor(m_actor_path.top());
	}
	m_actor_path.push(actor);

	m_spr_path.push(spr);
}

void CurrSprTreePath::Pop()
{
	assert(!m_spr_path.empty() && !m_actor_path.empty());
	m_spr_path.pop();
	m_actor_path.pop();
}

const s2::Sprite* CurrSprTreePath::TopSpr() const
{
	if (m_spr_path.empty()) {
		return NULL;
	} else {
		return m_spr_path.top();
	}
}

const s2::Actor* CurrSprTreePath::TopActor() const
{
	if (m_actor_path.empty()) {
		return NULL;
	} else {
		return m_actor_path.top();
	}
}

}