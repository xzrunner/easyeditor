#include "Symbol.h"
#include "FileIO.h"
#include "Sprite.h"

#include <ee/ObjectVector.h>
#include <ee/SpriteRenderer.h>

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

void Symbol::Draw(const s2::RenderParams& params, const ee::Sprite* spr) const
{
	m_anchors.DrawSprites(params);
	for (int i = 0, n = m_ext_refs.size(); i < n; ++i) {
		ee::SpriteRenderer::Draw(m_ext_refs[i], params);
	}
	m_anchors.DrawNodes(params);
}

void Symbol::ReloadTexture() const
{
}

sm::rect Symbol::GetSize(const ee::Sprite* sprite) const
{
	return sm::rect(sm::vec2(0, 0), m_width, m_height);
}

void Symbol::Traverse(ee::Visitor& visitor)
{
	m_anchors.Traverse(visitor);
	ee::ObjectVector<Sprite>::Traverse(m_ext_refs, visitor, ee::DT_ALL);
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
		ee::cu::RemoveRefFonctor<Sprite>());
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

void Symbol::LoadResources()
{
	FileIO::Load(m_filepath.c_str(), this);
}

}
}