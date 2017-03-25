#include "Symbol.h"
#include "FileIO.h"
#include "Sprite.h"

#include <ee/ObjectVector.h>
#include <ee/SpriteRenderer.h>
#include <ee/SymbolType.h>

#include <gum/FilepathHelper.h>

namespace eui
{
namespace window
{

Symbol::Symbol()
{

}

Symbol::~Symbol()
{

}

int Symbol::Type() const
{
	return ee::SYM_UIWND;
}

void Symbol::Draw(const s2::RenderParams& params, const s2::Sprite* spr) const
{
	m_anchors.DrawSprites(params);
	for (int i = 0, n = m_ext_refs.size(); i < n; ++i) {
		ee::SpriteRenderer::Instance()->Draw(m_ext_refs[i], params);
	}
	m_anchors.DrawNodes(params);
}

sm::rect Symbol::GetBounding(const s2::Sprite* spr, const s2::Actor* actor) const
{
	return sm::rect(sm::vec2(0, 0), m_width, m_height);
}

void Symbol::Traverse(ee::Visitor<ee::Sprite>& visitor)
{
	m_anchors.Traverse(visitor);

	std::vector<ee::Sprite*> ext_refs;
	copy(m_ext_refs.begin(), m_ext_refs.end(), back_inserter(ext_refs));
	ee::ObjectVector<ee::Sprite>::Traverse(ext_refs, visitor, ee::DT_ALL);
}

void Symbol::InsertExtRef(Sprite* spr)
{
	spr->AddReference();
	m_ext_refs.push_back(spr);
}

void Symbol::RemoveExtRef(Sprite* spr)
{
	std::vector<Sprite*>::iterator itr = m_ext_refs.begin();
	for ( ; itr != m_ext_refs.end(); ) {
		if (*itr == spr) {
			spr->RemoveReference();
			itr = m_ext_refs.erase(itr);
		} else {
			++itr;
		}
	}
}

void Symbol::ClearExtRef()
{
	for_each(m_ext_refs.begin(), m_ext_refs.end(), 
		cu::RemoveRefFunctor<Sprite>());
	m_ext_refs.clear();
}

void Symbol::ResetExtRefOrder(Sprite* spr, bool up)
{
	ee::ObjectVector<Sprite>::ResetOrder(m_ext_refs, spr, up);
}

void Symbol::ResetExtRefOrderMost(Sprite* spr, bool up)
{
	ee::ObjectVector<Sprite>::ResetOrderMost(m_ext_refs, spr, up);
}

bool Symbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath)) {
		return false;
	}
	FileIO::Load(m_filepath.c_str(), this);
	return true;
}

}
}