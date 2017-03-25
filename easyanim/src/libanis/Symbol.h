#ifndef _LIBANIS_SYMBOL_H_
#define _LIBANIS_SYMBOL_H_

#include <ee/Symbol.h>
#include <ee/SpriteLoader.h>

namespace libanim { class Symbol; }

namespace libanis
{

class Symbol : public ee::Symbol
{
public:
	Symbol();
	virtual ~Symbol();

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual void Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const s2::Sprite* spr = NULL, const s2::Actor* actor = NULL) const;

	/**
	 *  @interface
	 *    ee::Symbol
	 */
	virtual void ReloadTexture() const;

protected:
	virtual bool LoadResources();

private:
	class SpriteLoader : public ee::SpriteLoader
	{
	public:
		SpriteLoader(const std::string& dirpath) 
			: m_dirpath(dirpath) {}

	protected:
		virtual std::string GetSymbolPath(const std::string& dir, const Json::Value& val) const;

	private:
		std::string m_dirpath;

	}; // SpriteLoader

private:
	void Clear();

private:
	sm::rect m_rect;

	std::vector<libanim::Symbol*> m_syms;	

}; // Symbol

}

#endif // _LIBANIS_SYMBOL_H_