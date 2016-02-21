#ifndef _EASYBUILDER_CODE_STYLE_H_
#define _EASYBUILDER_CODE_STYLE_H_

#include <vadefs.h>
#include <stdarg.h>

namespace ebuilder
{

#define PARSER_PARAMS(n, str)			\
	std::string val;					\
	va_list vl;							\
	va_start(vl, (n));					\
	for (size_t i = 0; i < (n); ++i)	\
	{									\
		val = va_arg(vl, std::string);	\
		(str) += val;					\
		if (i != (n) - 1)				\
			(str) += ", ";				\
	}									\
	va_end(vl);							\

}

#endif // _EASYBUILDER_CODE_STYLE_H_
