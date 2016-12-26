#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _unirender_wrap_c_h_
#define _unirender_wrap_c_h_

#include <stdbool.h>

void ur_set_point_size(void* rc, float size);
void ur_set_line_width(void* rc, float width);
void ur_enable_line_stripple(void* rc, bool stripple);
void ur_set_line_stripple(void* rc, int pattern);

#endif // _unirender_wrap_c_h_

#ifdef __cplusplus
}
#endif