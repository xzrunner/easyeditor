#ifndef _LIBANIS_SYMBOL_H_
#define _LIBANIS_SYMBOL_H_

#include "../libanim/LayersLoader.h"

#include <drag2d.h>

namespace libanim { class Symbol; }

namespace libanis
{

class Symbol : public d2d::Symbol
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
	virtual void ReloadTexture() const;
	virtual void Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color = d2d::ColorTrans(), 
		const d2d::Sprite* spr = NULL, const d2d::Sprite* root = NULL) const;
	virtual d2d::Rect GetSize(const d2d::Sprite* sprite = NULL) const;

protected:
	virtual void LoadResources();

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