#ifndef _EASYUI_WINDOW_SYMBOL_H_
#define _EASYUI_WINDOW_SYMBOL_H_

#include "AnchorMgr.h"

#include <ee/Symbol.h>

namespace eui
{
namespace window
{

class Sprite;

class Symbol : public ee::Symbol
{
public:
	Symbol();
	virtual ~Symbol();

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual int Type() const override;
	virtual void Traverse(const s2::SymbolVisitor& visitor) override {}
	virtual s2::RenderReturn DrawTree(cooking::DisplayList* dlist, const s2::RenderParams& rp, const s2::Sprite* spr = nullptr) const override;

	/**
	 *  @interface
	 *    ee::Symbol
	 */
	virtual void Traverse(ee::RefVisitor<ee::Sprite>& visitor) override;

	AnchorMgr& GetAnchorMgr() { return m_anchors; }
	const AnchorMgr& GetAnchorMgr() const { return m_anchors; }

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }
	void SetWidth(int width) { m_width = width; }
	void SetHeight(int height) { m_height = height; } 

	void InsertExtRef(const std::shared_ptr<Sprite>& spr);
	void RemoveExtRef(const std::shared_ptr<Sprite>& spr);
	void ClearExtRef();
	void ResetExtRefOrder(const std::shared_ptr<Sprite>& spr, bool up);
	void ResetExtRefOrderMost(const std::shared_ptr<Sprite>& spr, bool up);

	auto& GetExtRefs() const { return m_ext_refs; }

	static ee::SymPtr Create() { return std::make_shared<Symbol>(); }

protected:
	virtual sm::rect GetBoundingImpl(const s2::Sprite* spr = nullptr, const s2::Actor* actor = NULL, bool cache = true) const override;

	virtual bool LoadResources() override;

private:
	int m_width, m_height;

	AnchorMgr m_anchors;

	std::string m_wrap_path;

	std::vector<std::shared_ptr<Sprite>> m_ext_refs;

}; // Symbol

}
}

#endif // _EASYUI_WINDOW_SYMBOL_H_