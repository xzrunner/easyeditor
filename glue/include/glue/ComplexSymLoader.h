#ifndef _GLUE_COMPLEX_SYM_LOADER_H_
#define _GLUE_COMPLEX_SYM_LOADER_H_

#include <CU_Uncopyable.h>

#include <json/json.h>

#include <string>

namespace s2 { class ComplexSymbol; }

namespace glue
{

class ComplexSymLoader : private cu::Uncopyable
{
public:
	ComplexSymLoader(s2::ComplexSymbol* sym);
	~ComplexSymLoader();

	void LoadJson(const std::string& filepath);

private:
	s2::ComplexSymbol* m_sym;

}; // ComplexSymLoader

}

#endif // _GLUE_COMPLEX_SYM_LOADER_H_