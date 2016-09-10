#ifndef _GUM_COMPLEX_SPR_LOADER_H_
#define _GUM_COMPLEX_SPR_LOADER_H_

#include <CU_Uncopyable.h>

#include <json/json.h>

namespace s2 { class ComplexSprite; }

namespace gum
{

class ComplexSprLoader : private cu::Uncopyable
{
public:
	ComplexSprLoader(s2::ComplexSprite* spr);
	~ComplexSprLoader();

	void LoadJson(const Json::Value& val, const std::string& dir);

private:
	s2::ComplexSprite* m_spr;

}; // ComplexSprLoader

}

#endif // _GUM_COMPLEX_SPR_LOADER_H_
