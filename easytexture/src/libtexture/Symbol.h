#ifndef _EASYTEXTURE_SYMBOL_H_
#define _EASYTEXTURE_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/TextureSymbol.h>

#include <vector>

namespace ee { class Shape; }

namespace etexture
{

class Symbol : public ee::Symbol, public s2::TextureSymbol
{
public:
	Symbol();
	virtual ~Symbol();

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual void Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const s2::Sprite* sprite = NULL) const;

	/**
	 *  @interface
	 *    ee::Symbol
	 */
	virtual void ReloadTexture() const;

	const std::vector<ee::Shape*>& GetAllShapes() const { return m_shapes; }

	static ee::Symbol* Create() { return new Symbol(); }
	
protected:
	virtual void LoadResources();

	void Clear();

private:
	std::vector<ee::Shape*> m_shapes;

	friend class SymbolContainer;

}; // Symbol

}

#endif // _EASYTEXTURE_SYMBOL_H_