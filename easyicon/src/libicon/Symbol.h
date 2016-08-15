#ifndef _EASYICON_SYMBOL_H_
#define _EASYICON_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/IconSymbol.h>

namespace ee { class Image; }

namespace eicon
{

class Icon;

class Symbol : public ee::Symbol, public s2::IconSymbol
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

	void SetIcon(Icon* icon);
	const Icon* GetIcon() const { return m_icon; }
	Icon* GetIcon() { return m_icon; }

	void SetImage(ee::Image* img);

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	Icon* m_icon;

}; // Symbol

}

#endif // _EASYICON_SYMBOL_H_