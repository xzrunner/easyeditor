
#ifndef LIBBUILDER_CODE_STYLE_H
#define LIBBUILDER_CODE_STYLE_H

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

#endif // LIBBUILDER_CODE_STYLE_H
