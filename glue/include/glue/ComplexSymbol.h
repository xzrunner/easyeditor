#ifndef _GLUE_COMPLEX_SYMBOL_H_
#define _GLUE_COMPLEX_SYMBOL_H_

#include <sprite2/ComplexSymbol.h>

#include <string>

namespace glue
{

class ComplexSymbol : public s2::ComplexSymbol
{
public:

	void LoadJson(const std::string& filepath);

}; // ComplexSymbol

}

#endif // _GLUE_COMPLEX_SYMBOL_H_