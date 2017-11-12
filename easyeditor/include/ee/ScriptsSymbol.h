#ifndef _EASYEDITOR_SCRIPTS_SYMBOL_H_
#define _EASYEDITOR_SCRIPTS_SYMBOL_H_

#include "Symbol.h"

namespace ee
{

class ScriptsSymbol : public Symbol
{
public:
	ScriptsSymbol() {}
	virtual ~ScriptsSymbol() {}

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual int Type() const override;
	virtual void Traverse(const s2::SymbolVisitor& visitor) override {}
	virtual s2::RenderReturn DrawTree(cooking::DisplayList* dlist, const s2::RenderParams& rp, const s2::Sprite* spr = nullptr) const override { return s2::RENDER_NO_DATA; }

	const std::string GetContent() const { return m_data; }

protected:
	virtual sm::rect GetBoundingImpl(const s2::Sprite* spr = NULL, const s2::Actor* actor = NULL, bool cache = true) const override {
		return sm::rect(0, 0); }

	virtual bool LoadResources() override;

private:
	std::string m_data;

}; // ScriptsSymbol

}

#endif // _EASYEDITOR_SCRIPTS_SYMBOL_H_