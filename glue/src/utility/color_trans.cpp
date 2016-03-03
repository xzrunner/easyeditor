//#include "color_trans.h"
//#include ""
//
//namespace glue
//{
//
//uint32_t color2int(const Colorf& col, PixelType type)
//{
//	uint32_t ret = 0;
//	if (type == PT_RGBA) {
//		ret = (color2int(col.r)) << 24 
//			| (color2int(col.g)) << 16
//			| (color2int(col.b)) << 8
//			| (color2int(col.a));
//	} else if (type == PT_ARGB) {
//		ret = (color2int(col.a)) << 24 
//			| (color2int(col.r)) << 16
//			| (color2int(col.g)) << 8
//			| (color2int(col.b));
//	} else if (type == PT_ABGR) {
//		ret = (color2int(col.a)) << 24 
//			| (color2int(col.b)) << 16
//			| (color2int(col.g)) << 8
//			| (color2int(col.r));
//	} else if (type == PT_BGRA) {
//		ret = (color2int(col.b)) << 24 
//			| (color2int(col.g)) << 16
//			| (color2int(col.r)) << 8
//			| (color2int(col.a));
//	}
//	return ret;
//}
//
//}