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

	//
	// Cloneable interface
	//
	virtual Symbol* Clone() const { return NULL; }

	//
	// Symbol interfaces
	//
	virtual void Draw(const s2::RenderParams& params, const ee::Sprite* spr = NULL) const;
	virtual void ReloadTexture() const;
	virtual sm::rect GetSize(const ee::Sprite* sprite = NULL) const;
	virtual void Traverse(ee::Visitor& visitor);

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
	virtual void LoadResources();

private:
	int m_width, m_height;

	AnchorMgr m_anchors;

	std::string m_wrap_path;

	std::vector<Sprite*> m_ext_refs;

}; // Symbol

}
}

#endif // _EASYUI_WINDOW_SYMBOL_H_