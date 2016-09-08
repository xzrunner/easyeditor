#ifndef _GLUE_COMPLEX_SPR_LOADER_H_
#define _GLUE_COMPLEX_SPR_LOADER_H_

#include <CU_Uncopyable.h>

#include <json/json.h>

namespace s2 { class ComplexSprite; }

namespace glue
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

#endif // _GLUE_COMPLEX_SPR_LOADER_H_