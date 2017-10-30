#include "CurrSprTreePath.h"

#include <sprite2/Sprite.h>

#include <assert.h>

namespace ee
{

CU_SINGLETON_DEFINITION(CurrSprTreePath);

CurrSprTreePath::CurrSprTreePath()
{
	m_spr_path.push(nullptr);
	m_actor_path.push(nullptr);
}

void CurrSprTreePath::Push(const s2::SprPtr& spr)
{
	s2::ActorPtr actor = nullptr;
	if (!m_actor_path.empty()) {
		actor = spr->QueryActorRef(m_actor_path.top().get());
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

s2::SprPtr CurrSprTreePath::TopSpr() const
{
	if (m_spr_path.empty()) {
		return nullptr;
	} else {
		return m_spr_path.top();
	}
}

s2::ActorPtr CurrSprTreePath::TopActor() const
{
	if (m_actor_path.empty()) {
		return nullptr;
	} else {
		return m_actor_path.top();
	}
}

}