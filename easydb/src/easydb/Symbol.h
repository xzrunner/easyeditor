#ifndef _EASYDB_SYMBOL_H_
#define _EASYDB_SYMBOL_H_

#include <ee/Symbol.h>

namespace gum { class Image; }

namespace edb
{

class Database;
class Sprite;

class Symbol : public ee::Symbol
{
public:
	Symbol(const Database* db);
	virtual ~Symbol();

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual int Type() const override;
	virtual void Traverse(const s2::SymbolVisitor& visitor) override {}
	virtual s2::RenderReturn DrawTree(cooking::DisplayList* dlist, const s2::RenderParams& rp, const s2::Sprite* spr = nullptr) const override;

	const Database* GetDB() const { return m_db; }

	static void OnSprPressed(Sprite* spr, const sm::vec2& pos);

protected:
	virtual sm::rect GetBoundingImpl(const s2::Sprite* spr = NULL, const s2::Actor* actor = NULL, bool cache = true) const override;

	virtual bool LoadResources() override { return false; }

private:
	static void DrawBG(const S2_MAT& mt);
	static void DrawBtn(const S2_MAT& mt, const sm::vec2& offset, bool closed);
	static void DrawSnapshoot(const S2_MAT& mt, const gum::Image* img);

private:
	const Database* m_db;

}; // Symbol

}

#endif // _EASYDB_SYMBOL_H_