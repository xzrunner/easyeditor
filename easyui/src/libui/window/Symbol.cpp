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

s2::RenderReturn Symbol::DrawTree(cooking::DisplayList* dlist, const s2::RenderParams& rp, const s2::Sprite* spr) const
{
	m_anchors.DrawSprites(rp);
	for (int i = 0, n = m_ext_refs.size(); i < n; ++i) {
		ee::SpriteRenderer::Instance()->Draw(m_ext_refs[i].get(), rp);
	}
	m_anchors.DrawNodes(rp);
	return s2::RENDER_OK;
}

sm::rect Symbol::GetBoundingImpl(const s2::Sprite* spr, const s2::Actor* actor, bool cache) const
{
	return sm::rect(sm::vec2(0, 0), m_width, m_height);
}

void Symbol::Traverse(ee::RefVisitor<ee::Sprite>& visitor)
{
	m_anchors.Traverse(visitor);

	std::vector<ee::SprPtr> ext_refs;
	copy(m_ext_refs.begin(), m_ext_refs.end(), back_inserter(ext_refs));
	ee::ObjectVector<ee::Sprite>::Traverse(ext_refs, visitor, ee::DT_ALL);
}

void Symbol::InsertExtRef(const std::shared_ptr<Sprite>& spr)
{
	m_ext_refs.push_back(spr);
}

void Symbol::RemoveExtRef(const std::shared_ptr<Sprite>& spr)
{
	auto itr = m_ext_refs.begin();
	for ( ; itr != m_ext_refs.end(); ) {
		if (*itr == spr) {
			itr = m_ext_refs.erase(itr);
		} else {
			++itr;
		}
	}
}

void Symbol::ClearExtRef()
{
	m_ext_refs.clear();
}

void Symbol::ResetExtRefOrder(const std::shared_ptr<Sprite>& spr, bool up)
{
	ee::ObjectVector<Sprite>::ResetOrder(m_ext_refs, spr, up);
}

void Symbol::ResetExtRefOrderMost(const std::shared_ptr<Sprite>& spr, bool up)
{
	ee::ObjectVector<Sprite>::ResetOrderMost(m_ext_refs, spr, up);
}

bool Symbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath.c_str())) {
		return false;
	}
	FileIO::Load(m_filepath.c_str(), *this);
	return true;
}

}
}