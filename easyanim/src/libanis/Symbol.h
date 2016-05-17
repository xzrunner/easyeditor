#ifndef _LIBANIS_SYMBOL_H_
#define _LIBANIS_SYMBOL_H_

#include "../libanim/LayersLoader.h"



namespace eanim { class Symbol; }

namespace libanis
{

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

	std::vector<eanim::Symbol*> m_symbols;	

}; // Symbol

}

#endif // _LIBANIS_SYMBOL_H_