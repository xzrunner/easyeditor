#ifndef _LIBANIM_SYMBOL_H_
#define _LIBANIM_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/AnimSymbol.h>

#include <vector>

namespace s2loader { class SpriteLoader; }

namespace libanim
{

class SpriteLoader;
class Sprite;

class Symbol : public ee::Symbol, public s2::AnimSymbol
{
public:
	Symbol();

	/**
	 *  @interface
	 *    ee::Symbol
	 */
	virtual void ReloadTexture() const override;
	virtual void Traverse(ee::RefVisitor<ee::Sprite>& visitor) override;

	void Load(const std::shared_ptr<s2loader::SpriteLoader>& spr_loader);

	static ee::SymPtr Create() { return std::make_shared<Symbol>(); }

protected:
	virtual bool LoadResources() override;

private:
	void LoadEE();

}; // Symbol

}

#endif // _LIBANIM_SYMBOL_H_