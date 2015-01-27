#ifndef _D2D_MATH_COMMON_H_
#define _D2D_MATH_COMMON_H_

///This function gets the first power of 2 >= the
///int that we pass it.
inline int next_p2(int a)
{
	int rval = 1;
	while(rval < a) {
		rval <<= 1;
	}
	return rval;
}

inline bool is_power_of_two(int x)
{
	return (x != 0) && ((x & (x - 1)) == 0);
}

#endif // _D2D_MATH_COMMON_H_