#ifndef _LIBANIS_SYMBOL_H_
#define _LIBANIS_SYMBOL_H_

#include "../libanim/LayersLoader.h"

#include <sprite2/DummySymbol.h>

namespace eanim { class Symbol; }

namespace libanis
{

class Symbol : public ee::Symbol, public s2::DummySymbol
{
public:
	Symbol();
	virtual ~Symbol();

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual void Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const s2::Sprite* spr = NULL) const;

	/**
	 *  @interface
	 *    ee::Symbol
	 */
	virtual void ReloadTexture() const;

protected:
	virtual void LoadResources();

private:
	class Loader : public eanim::LayersLoader
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
	sm::rect m_rect;

	std::vector<eanim::Symbol*> m_syms;	

}; // Symbol

}

#endif // _LIBANIS_SYMBOL_H_