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
	virtual s2::RenderReturn DrawTree(cooking::DisplayList* dlist, const s2::RenderParams& rp, const s2::Sprite* spr = nullptr) const override;

	/**
	 *  @interface
	 *    ee::Symbol
	 */
	virtual void ReloadTexture() const override;

protected:
	virtual sm::rect GetBoundingImpl(const s2::Sprite* spr = NULL, const s2::Actor* actor = NULL, bool cache = true) const override;

	virtual bool LoadResources() override;

private:
	class SpriteLoader : public ee::SpriteLoader
	{
	public:
		SpriteLoader(const std::string& dirpath) 
			: m_dirpath(dirpath) {}

	protected:
		virtual std::string GetSymbolPath(const std::string& dir, const Json::Value& val) const override;

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