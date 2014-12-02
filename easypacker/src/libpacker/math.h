#ifndef _LIBPACKER_MATH_H_
#define _LIBPACKER_MATH_H_

namespace libpacker
{

///This function gets the first power of 2 >= the
///int that we pass it.
int next_p2(int a)
{
	int rval = 1;
	while(rval < a) {
		rval <<= 1;
	}
	return rval;
}

}

#endif // _LIBPACKER_MATH_H_