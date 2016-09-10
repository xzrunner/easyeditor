#ifndef _GUM_SCALE9_SYM_LOADER_H_
#define _GUM_SCALE9_SYM_LOADER_H_

#include <CU_Uncopyable.h>

#include <json/json.h>

#include <string>

namespace s2 { class Scale9Symbol; class Sprite; }

namespace gum
{

class Scale9SymLoader : private cu::Uncopyable
{
public:
	Scale9SymLoader(s2::Scale9Symbol* sym);
	~Scale9SymLoader();

	void LoadJson(const std::string& filepath);

protected:
	virtual s2::Sprite* LoadSprite(const Json::Value& val, const std::string& dir);

private:
	s2::Scale9Symbol* m_sym;

}; // Scale9SymLoader

}

#endif // _GUM_SCALE9_SYM_LOADER_H_
