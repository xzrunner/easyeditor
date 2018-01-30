#ifndef _S2LOADER_COMPLEX_SPR_LOADER_H_
#define _S2LOADER_COMPLEX_SPR_LOADER_H_

#include <cu/uncopyable.h>
#include <cu/cu_stl.h>

#include <json/json.h>

namespace s2 { class ComplexSprite; }
namespace simp { class NodeComplexSpr; }

namespace s2loader
{

class ComplexSprLoader : private cu::Uncopyable
{
public:
	ComplexSprLoader(s2::ComplexSprite& spr);

	void LoadJson(const Json::Value& val, const CU_STR& dir);
	void LoadBin(const simp::NodeComplexSpr* node);

private:
	s2::ComplexSprite& m_spr;

}; // ComplexSprLoader

}

#endif // _S2LOADER_COMPLEX_SPR_LOADER_H_
