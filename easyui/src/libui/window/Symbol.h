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
	virtual int Type() const;
	virtual void Traverse(const s2::SymbolVisitor& visitor) {}
	virtual void Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const s2::Sprite* spr = NULL) const;

	/**
	 *  @interface
	 *    ee::Symbol
	 */
	virtual void Traverse(ee::Visitor<ee::Sprite>& visitor);

	AnchorMgr& GetAnchorMgr() { return m_anchors; }
	const AnchorMgr& GetAnchorMgr() const { return m_anchors; }

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }
	void SetWidth(int width) { m_width = width; }
	void SetHeight(int height) { m_height = height; } 

	void InsertExtRef(Sprite* spr);
	void RemoveExtRef(Sprite* spr);
	void ClearExtRef();
	void ResetExtRefOrder(Sprite* spr, bool up);
	void ResetExtRefOrderMost(Sprite* spr, bool up);

	const std::vector<Sprite*>& GetExtRefs() const { return m_ext_refs; }

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual bool LoadResources();

private:
	int m_width, m_height;

	AnchorMgr m_anchors;

	std::string m_wrap_path;

	std::vector<Sprite*> m_ext_refs;

}; // Symbol

}
}

#endif // _EASYUI_WINDOW_SYMBOL_H_