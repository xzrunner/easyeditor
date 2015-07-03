#ifndef _LIBANIS_SYMBOL_H_
#define _LIBANIS_SYMBOL_H_

#include "../libanim/LayersLoader.h"

#include <drag2d.h>

namespace libanim { class Symbol; }

namespace libanis
{

class Symbol : public d2d::ISymbol
{
public:
	Symbol();
	virtual ~Symbol();

	//
	// ICloneable interface
	//
	virtual Symbol* clone() const { return NULL; }

	//
	// ISymbol interfaces
	//
	virtual void reloadTexture() const;
	virtual void draw(const d2d::Matrix& mt,
		const d2d::Colorf& mul = d2d::Colorf(1, 1, 1, 1), 
		const d2d::Colorf& add = d2d::Colorf(0, 0, 0, 0),
		const d2d::Colorf& r_trans = d2d::Colorf(1, 0, 0, 0),
		const d2d::Colorf& g_trans = d2d::Colorf(0, 1, 0, 0),
		const d2d::Colorf& b_trans = d2d::Colorf(0, 0, 1, 0),
		const d2d::ISprite* sprite = NULL) const;
	virtual d2d::Rect getSize(const d2d::ISprite* sprite = NULL) const;

protected:
	virtual void loadResources();

private:
	class Loader : public libanim::LayersLoader
	{
	public:
		Loader(const std::string& dirpath) : m_dirpath(dirpath) {}

	protected:
		virtual std::string GetSymbolPath(const std::string& dir, 
			const Json::Value& json_val) const;

	private:
		std::string m_dirpath;

	}; // Loader

private:
	void Clear();

private:
	d2d::Rect m_rect;

	std::vector<libanim::Symbol*> m_symbols;	

}; // Symbol

}

#endif // _LIBANIS_SYMBOL_H_